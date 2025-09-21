#pragma once

namespace Game {
    void set_vulkan_object_name(VkDevice device, void* object, VkObjectType object_type, const char* object_name);

    void begin_cmd_debug_label(VkDevice device, VkCommandBuffer command_buffer, const VkDebugUtilsLabelEXT& label);

    void insert_cmd_debug_label(VkDevice device, VkCommandBuffer command_buffer, const VkDebugUtilsLabelEXT& label);

    void end_cmd_debug_label(VkDevice device, VkCommandBuffer command_buffer);

    void begin_queue_debug_label(VkDevice device, VkQueue queue, const VkDebugUtilsLabelEXT& label);

    void insert_queue_debug_label(VkDevice device, VkQueue queue, const VkDebugUtilsLabelEXT& label);

    void end_queue_debug_label(VkDevice device, VkQueue queue);
}
