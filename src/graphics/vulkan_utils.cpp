#include "vulkan_utils.h"

namespace Game {
    void set_vulkan_object_name(VkDevice device, void* object, VkObjectType object_type, const char* object_name) {
        VkDebugUtilsObjectNameInfoEXT name_info{};
        name_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        name_info.objectType = object_type;
        name_info.objectHandle = (uint64_t) object;
        name_info.pObjectName = object_name;

        auto set_object_name_fn = (PFN_vkSetDebugUtilsObjectNameEXT) vkGetDeviceProcAddr(device, "vkSetDebugUtilsObjectNameEXT");
        if (!set_object_name_fn) {
            GM_THROW("Could not get function to set Vulkan object name");
        }

        set_object_name_fn(device, &name_info);
    }

    void begin_cmd_debug_label(VkDevice device, VkCommandBuffer command_buffer, const VkDebugUtilsLabelEXT& label) {
        auto begin_cmd_debug_label_fn = (PFN_vkCmdBeginDebugUtilsLabelEXT) vkGetDeviceProcAddr(device, "vkCmdBeginDebugUtilsLabelEXT");
        if (!begin_cmd_debug_label_fn) {
            GM_THROW("Could not get function to begin cmd debug label [" << label.pLabelName << "]");
        }
        begin_cmd_debug_label_fn(command_buffer, &label);
    }

    void insert_cmd_debug_label(VkDevice device, VkCommandBuffer command_buffer, const VkDebugUtilsLabelEXT& label) {
        auto insert_cmd_debug_label_fn = (PFN_vkCmdInsertDebugUtilsLabelEXT) vkGetDeviceProcAddr(device, "vkCmdInsertDebugUtilsLabelEXT");
        if (!insert_cmd_debug_label_fn) {
            GM_THROW("Could not get function to insert cmd debug label [" << label.pLabelName << "]");
        }
        insert_cmd_debug_label_fn(command_buffer, &label);
    }

    void end_cmd_debug_label(VkDevice device, VkCommandBuffer command_buffer) {
        auto end_cmd_debug_label_fn = (PFN_vkCmdEndDebugUtilsLabelEXT) vkGetDeviceProcAddr(device, "vkCmdEndDebugUtilsLabelEXT");
        if (!end_cmd_debug_label_fn) {
            GM_THROW("Could not get function to end cmd debug label");
        }
        end_cmd_debug_label_fn(command_buffer);
    }

    void begin_queue_debug_label(VkDevice device, VkQueue queue, const VkDebugUtilsLabelEXT& label) {
        auto begin_queue_debug_label_fn = (PFN_vkQueueBeginDebugUtilsLabelEXT) vkGetDeviceProcAddr(device, "vkQueueBeginDebugUtilsLabelEXT");
        if (!begin_queue_debug_label_fn) {
            GM_THROW("Could not get function to begin queue debug label [" << label.pLabelName << "]");
        }
        begin_queue_debug_label_fn(queue, &label);
    }

    void insert_queue_debug_label(VkDevice device, VkQueue queue, const VkDebugUtilsLabelEXT& label) {
        auto insert_queue_debug_label_fn = (PFN_vkQueueInsertDebugUtilsLabelEXT) vkGetDeviceProcAddr(device, "vkQueueInsertDebugUtilsLabelEXT");
        if (!insert_queue_debug_label_fn) {
            GM_THROW("Could not get function to insert queue debug label [" << label.pLabelName << "]");
        }
        insert_queue_debug_label_fn(queue, &label);
    }

    void end_queue_debug_label(VkDevice device, VkQueue queue) {
        auto end_queue_debug_label_fn = (PFN_vkQueueEndDebugUtilsLabelEXT) vkGetDeviceProcAddr(device, "vkQueueEndDebugUtilsLabelEXT");
        if (!end_queue_debug_label_fn) {
            GM_THROW("Could not get function to end queue debug label");
        }
        end_queue_debug_label_fn(queue);
    }
}