#include "renderer.h"

#include "vulkan_swap_chain.h"

namespace Game {
    void wait_until_window_is_not_minimized(const Window& window) {
        bool iconified = is_window_iconified(window);

        i32 width = 0;
        i32 height = 0;
        get_window_size(window, &width, &height);

        while (iconified || width == 0 || height == 0) {
            iconified = is_window_iconified(window);
            get_window_size(window, &width, &height);
            glfwWaitEvents();
        }
    }

    void recreate_swap_chain(Vulkan& vulkan) {
        wait_until_window_is_not_minimized(*vulkan.config.window);
        vkDeviceWaitIdle(vulkan.device);
        recreate_vulkan_swap_chain(vulkan, {
            .window = vulkan.config.window,
            .name = vulkan.swap_chain_name,
            .image_count = vulkan.config.max_frames_in_flight,
        });
    }

    void record_command_buffer(Vulkan& vulkan, VkCommandBuffer command_buffer) {
        VkCommandBufferBeginInfo command_buffer_begin_info{};
        command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        command_buffer_begin_info.flags = 0;
        command_buffer_begin_info.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info) != VK_SUCCESS) {
            GM_THROW("Could not begin command buffer");
        }

        begin_cmd_debug_label(vulkan.device, command_buffer, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "CommandBuffer",
        });

        VkClearColorValue clear_color_value = {
            .float32 = {0.0f, 0.0f, 0.0f, 1.0f}
        };
        VkClearValue clear_color = {
            .color = clear_color_value
        };

        VkRenderPassBeginInfo render_pass_begin_info{};
        render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_begin_info.renderPass = vulkan.swap_chain_render_pass;
        render_pass_begin_info.framebuffer = vulkan.swap_chain_framebuffers[vulkan.swap_chain_current_image_index];
        render_pass_begin_info.renderArea.offset = {0, 0};
        render_pass_begin_info.renderArea.extent = vulkan.swap_chain_extent;
        render_pass_begin_info.clearValueCount = 1;
        render_pass_begin_info.pClearValues = &clear_color;

        insert_cmd_debug_label(vulkan.device, command_buffer, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "Begin render pass",
        });

        vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        insert_cmd_debug_label(vulkan.device, command_buffer, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "Bind pipeline",
        });

        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan.pipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(vulkan.swap_chain_extent.width);
        viewport.height = static_cast<float>(vulkan.swap_chain_extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        insert_cmd_debug_label(vulkan.device, command_buffer, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "Set viewport",
        });

        u32 first_viewport = 0;
        u32 viewport_count = 1;
        vkCmdSetViewport(command_buffer, first_viewport, viewport_count, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = vulkan.swap_chain_extent;

        insert_cmd_debug_label(vulkan.device, command_buffer, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "Set scissor",
        });

        u32 first_scissor = 0;
        u32 scissor_count = 1;
        vkCmdSetScissor(command_buffer, first_scissor, scissor_count, &scissor);

        insert_cmd_debug_label(vulkan.device, command_buffer, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "Draw",
        });

        u32 vertex_count = 3; // Even though we don't have a vertex buffer, we technically still have 3 vertices to draw.
        u32 instance_count = 1; // Used for instanced rendering, use 1 if you're not doing that.
        u32 first_vertex = 0; // Used as an offset into the vertex buffer, defines the lowest value of gl_VertexIndex.
        u32 first_instance = 0; // Used as an offset for instanced rendering, defines the lowest value of gl_InstanceIndex.
        vkCmdDraw(command_buffer, vertex_count, instance_count, first_vertex, first_instance);

        insert_cmd_debug_label(vulkan.device, command_buffer, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "End render pass",
        });

        vkCmdEndRenderPass(command_buffer);

        end_cmd_debug_label(vulkan.device, command_buffer);

        if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
            GM_THROW("Could not end command buffer");
        }
    }

    void render_frame(Renderer& renderer) {
        Vulkan& vulkan = renderer.vulkan;

        VkCommandBuffer command_buffer = vulkan.command_buffers[renderer.current_frame];
        VkFence in_flight_fence = vulkan.swap_chain_in_flight_fences[renderer.current_frame];
        VkSemaphore image_available_semaphore = vulkan.swap_chain_image_available_semaphores[renderer.current_frame];
        VkSemaphore render_finished_semaphore = vulkan.swap_chain_render_finished_semaphores[renderer.current_frame];

        //
        // Wait for the previous frame to finish.
        //

        u32 fence_count = 1;
        VkBool32 wait_for_all_fences = VK_TRUE;
        u64 wait_for_fences_timeout = UINT64_MAX; // Wait forever until the fence is signaled.
        if (vkWaitForFences(vulkan.device, fence_count, &in_flight_fence, wait_for_all_fences, wait_for_fences_timeout) != VK_SUCCESS) {
            GM_THROW("Could not wait for 'in flight' fence for frame [" << renderer.current_frame << "]");
        }

        //
        // Acquire an image from the swap chain to use for the current frame.
        //

        u64 next_image_timeout = UINT64_MAX; // Wait forever until an image becomes available.
        VkFence image_available_fence = VK_NULL_HANDLE; // No fences to signal when an image becomes available.
        VkResult next_image_result = vkAcquireNextImageKHR(
            vulkan.device,
            vulkan.swap_chain,
            next_image_timeout,
            image_available_semaphore, // Signal that an image is available.
            image_available_fence,
            &vulkan.swap_chain_current_image_index
        );
        // VK_ERROR_OUT_OF_DATE_KHR: The swap chain has become incompatible with the surface and can no longer be used for rendering.
        if (next_image_result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreate_swap_chain(vulkan);
            return;
        }
        // VK_SUBOPTIMAL_KHR: The swap chain can still be used to successfully present to the surface, but the surface properties are no longer matched exactly.
        if (next_image_result != VK_SUCCESS && next_image_result != VK_SUBOPTIMAL_KHR) {
            GM_THROW("Could not acquire swap chain image for frame [" << renderer.current_frame << "]");
        }

        // Delay resetting the fence until after we have successfully acquired an image, and we know for sure we will be submitting work with it.
        // Thus, if we return early, the fence is still signaled and vkWaitForFences won't deadlock the next time we use the same fence object.
        if (vkResetFences(vulkan.device, fence_count, &in_flight_fence) != VK_SUCCESS) {
            GM_THROW("Could not reset 'in flight' fence for frame [" << renderer.current_frame << "]");
        }

        //
        // Record rendering commands to render the scene onto the swap chain image.
        //

        // Reset the command buffer to make sure it's ready for new rendering commands
        VkCommandBufferResetFlags command_buffer_reset_flags = 0;
        vkResetCommandBuffer(command_buffer, command_buffer_reset_flags);

        record_command_buffer(renderer.vulkan, command_buffer);

        //
        // Submit the rendering commands to the graphics queue to perform the rendering.
        //

        VkDebugUtilsLabelEXT graphis_queue_label{};
        graphis_queue_label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        graphis_queue_label.pLabelName = "GraphicsQueue";

        constexpr VkPipelineStageFlags color_output_pipeline_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &image_available_semaphore; // Wait until an image is available.
        submit_info.pWaitDstStageMask = &color_output_pipeline_stage; // Wait at the color output pipeline stage.
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &render_finished_semaphore; // Signal that the image is rendered and ready for presentation.

        begin_queue_debug_label(vulkan.device, vulkan.graphics_queue, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "GraphicsQueue",
        });

        insert_queue_debug_label(vulkan.device, vulkan.graphics_queue, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "Submit render commands",
        });

        u32 submit_count = 1;
        if (vkQueueSubmit(vulkan.graphics_queue, submit_count, &submit_info, in_flight_fence) != VK_SUCCESS) {
            GM_THROW("Could not submit render commands to graphics queue");
        }

        end_queue_debug_label(vulkan.device, vulkan.graphics_queue);

        //
        // Present the rendered swap chain image to the surface (screen).
        //

        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &render_finished_semaphore; // Wait until the image is rendered.
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &vulkan.swap_chain;
        present_info.pImageIndices = &vulkan.swap_chain_current_image_index;
        present_info.pResults = nullptr;

        begin_queue_debug_label(vulkan.device, vulkan.graphics_queue, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "PresentQueue",
        });

        insert_queue_debug_label(vulkan.device, vulkan.graphics_queue, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "Present swap chain image to the surface",
        });

        VkResult present_result = vkQueuePresentKHR(vulkan.present_queue, &present_info);

        // VK_ERROR_OUT_OF_DATE_KHR: The swap chain has become incompatible with the surface and can no longer be used for rendering.
        // VK_SUBOPTIMAL_KHR: The swap chain can still be used to successfully present to the surface, but the surface properties are no longer matched exactly.
        if (present_result == VK_ERROR_OUT_OF_DATE_KHR || present_result == VK_SUBOPTIMAL_KHR || renderer.framebuffer_resized) {
            renderer.framebuffer_resized = false;
            recreate_swap_chain(vulkan);
        } else if (present_result != VK_SUCCESS) {
            GM_THROW("Could not present swap chain image to the surface");
        }

        end_queue_debug_label(vulkan.device, vulkan.present_queue);

        renderer.current_frame = (renderer.current_frame + 1) % renderer.max_frames_in_flight;
    }

    bool handle_renderer_event(Renderer& renderer, const Event& event) {
        if (event.type == EventType::WindowResize) {
            renderer.framebuffer_resized = true;
            return true;
        }
        return false;
    }

    void create_renderer(Renderer& renderer, const RendererConfig& config) {
        renderer.max_frames_in_flight = config.max_frames_in_flight;

        create_vulkan(renderer.vulkan, {
            .window = config.window,
            .application_name = config.app_name,
            .engine_name = std::format("{} Engine", config.app_name),
            .validation_layers_enabled = config.debug_enabled,
            .max_frames_in_flight = config.max_frames_in_flight,
        });
    }

    void destroy_renderer(const Renderer& renderer) {
        vkDeviceWaitIdle(renderer.vulkan.device);
        destroy_vulkan(renderer.vulkan);
    }
}
