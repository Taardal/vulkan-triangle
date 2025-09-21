#include "vulkan.h"

#include "vulkan_device.h"
#include "vulkan_instance.h"
#include "vulkan_physical_device.h"
#include "vulkan_pipeline.h"
#include "vulkan_surface.h"
#include "vulkan_swap_chain.h"

namespace Game {
    void create_command_pool(Vulkan& vulkan) {
        const QueueFamilyIndices& queue_family_indices = vulkan.physical_device_queue_family_indices;

        VkCommandPoolCreateInfo command_pool_create_info{};
        command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        command_pool_create_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

        if (vkCreateCommandPool(vulkan.device, &command_pool_create_info, GM_VK_ALLOCATOR, &vulkan.command_pool) != VK_SUCCESS) {
            GM_THROW("Could not create command pool");
        }
    }

    void destroy_command_pool(const Vulkan& vulkan) {
        vkDestroyCommandPool(vulkan.device, vulkan.command_pool, GM_VK_ALLOCATOR);
    }

    void allocate_command_buffer(Vulkan& vulkan) {
        VkCommandBufferAllocateInfo command_buffer_allocate_info{};
        command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        command_buffer_allocate_info.commandPool = vulkan.command_pool;
        command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        command_buffer_allocate_info.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(vulkan.device, &command_buffer_allocate_info, &vulkan.command_buffer) != VK_SUCCESS) {
            GM_THROW("Could not allocate command buffer");
        }
    }

    void record_command_buffer(Vulkan& vulkan, VkCommandBuffer command_buffer, u32 image_index) {
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
        render_pass_begin_info.framebuffer = vulkan.swap_chain_framebuffers[image_index];
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

    void render_frame(Vulkan& vulkan) {
        i32 fence_count = 1;

        VkBool32 wait_for_all_fences = VK_TRUE;
        u64 wait_for_fences_timeout = UINT64_MAX;
        vkWaitForFences(vulkan.device, fence_count, &vulkan.in_flight_fence, wait_for_all_fences, wait_for_fences_timeout);

        vkResetFences(vulkan.device, fence_count, &vulkan.in_flight_fence);

        uint32_t image_index;
        u64 wait_for_next_image_timeout = UINT64_MAX;
        VkFence next_image_fence = VK_NULL_HANDLE;
        vkAcquireNextImageKHR(vulkan.device, vulkan.swap_chain, wait_for_next_image_timeout, vulkan.image_available_semaphore, next_image_fence, &image_index);

        VkCommandBufferResetFlags command_buffer_reset_flags = 0;
        vkResetCommandBuffer(vulkan.command_buffer, command_buffer_reset_flags);

        record_command_buffer(vulkan, vulkan.command_buffer, image_index);

        VkDebugUtilsLabelEXT graphis_queue_label{};
        graphis_queue_label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        graphis_queue_label.pLabelName = "GraphicsQueue";

        VkSemaphore wait_semaphores[] = {
            vulkan.image_available_semaphore
        };
        VkPipelineStageFlags wait_stages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
        };
        VkSemaphore signal_semaphores[] = {
            vulkan.render_finished_semaphore
        };

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphores;
        submit_info.pWaitDstStageMask = wait_stages;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &vulkan.command_buffer;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_semaphores;

        begin_queue_debug_label(vulkan.device, vulkan.graphics_queue, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "GraphicsQueue",
        });

        insert_queue_debug_label(vulkan.device, vulkan.graphics_queue, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "Submit render commands",
        });

        u32 submit_count = 1;
        if (vkQueueSubmit(vulkan.graphics_queue, submit_count, &submit_info, vulkan.in_flight_fence) != VK_SUCCESS) {
            GM_THROW("Could not submit render commands to graphics queue");
        }

        end_queue_debug_label(vulkan.device, vulkan.graphics_queue);

        VkSwapchainKHR swap_chains[] = {
            vulkan.swap_chain
        };

        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = signal_semaphores;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swap_chains;
        present_info.pImageIndices = &image_index;
        present_info.pResults = nullptr;

        begin_queue_debug_label(vulkan.device, vulkan.graphics_queue, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "PresentQueue",
        });

        insert_queue_debug_label(vulkan.device, vulkan.graphics_queue, VkDebugUtilsLabelEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName = "Present rendered image to swap chain",
        });

        vkQueuePresentKHR(vulkan.present_queue, &present_info);

        end_queue_debug_label(vulkan.device, vulkan.present_queue);
    }

    void create_vulkan(Vulkan& vulkan, const VulkanConfig& config) {
        if (glfwVulkanSupported() != GLFW_TRUE) {
            GM_THROW("Vulkan is not supported");
        }

        create_vulkan_instance(vulkan, {
            .application_name = config.application_name,
            .engine_name = config.engine_name,
            .validation_layers_enabled = config.validation_layers_enabled,
            .debug_extension_enabled = config.debug_extension_enabled,
        });

        create_vulkan_surface(vulkan, {
            .window = config.window,
        });

        pick_vulkan_physical_device(vulkan);

        create_vulkan_device(vulkan, {
            .name = "Device",
        });

        create_vulkan_swap_chain(vulkan, {
            .name = "SwapChain",
            .window = config.window,
        });

        create_vulkan_pipeline(vulkan, {
            .name = "TrianglePipeline",
            .vertex_shader_path = "res/shaders/triangle.vert.spv",
            .fragment_shader_path = "res/shaders/triangle.frag.spv",
        });

        create_command_pool(vulkan);
        allocate_command_buffer(vulkan);
    }

    void destroy_vulkan(const Vulkan& vulkan) {
        vkDeviceWaitIdle(vulkan.device);

        destroy_command_pool(vulkan);
        destroy_vulkan_pipeline(vulkan);
        destroy_vulkan_swap_chain(vulkan);
        destroy_vulkan_device(vulkan);
        destroy_vulkan_surface(vulkan);
        destroy_vulkan_instance(vulkan);
    }
}
