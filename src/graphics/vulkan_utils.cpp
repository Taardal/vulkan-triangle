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
}