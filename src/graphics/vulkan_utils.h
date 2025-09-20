#pragma once

namespace Game {
    void set_vulkan_object_name(VkDevice device, void* object, VkObjectType object_type, const char* object_name);
}