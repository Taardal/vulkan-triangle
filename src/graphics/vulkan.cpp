#include "vulkan.h"

namespace Game {
    Vulkan create_vulkan(const VulkanConfig& config) {
        Vulkan vulkan{};
        vulkan.config = config;

        if (!is_vulkan_supported()) {
            GM_THROW("Vulkan is not supported");
        }

        if (create_vulkan_instance(vulkan) != VK_SUCCESS) {
            GM_THROW("Could not create Vulkan instance");
        }

        if (vulkan.config.validation_layers_enabled) {
            if (create_vulkan_debug_messenger(vulkan) != VK_SUCCESS) {
                GM_THROW("Could not create Vulkan debug messenger");
            }
        }

        if (create_vulkan_surface(vulkan) != VK_SUCCESS) {
            GM_THROW("Could not create Vulkan surface");
        }

        pick_physical_device(vulkan);
        create_device(vulkan);

        return vulkan;
    }

    void destroy_vulkan(Vulkan& vulkan) {
        destroy_device(vulkan);
        destroy_vulkan_surface(vulkan);
        if (vulkan.config.validation_layers_enabled) {
            destroy_vulkan_debug_messenger(vulkan);
        }
        destroy_vulkan_instance(vulkan);
    }

    VkResult create_vulkan_instance(Vulkan& vulkan) {
        vulkan.extensions = get_required_vulkan_extensions(vulkan);
        if (!has_vulkan_extensions(vulkan.extensions)) {
            GM_THROW("System does not have required Vulkan extensions");
        }

        if (vulkan.config.validation_layers_enabled) {
            vulkan.validation_layers = get_required_vulkan_validation_layers();
            if (!has_vulkan_validation_layers(vulkan.validation_layers)) {
                GM_THROW("System does not have required Vulkan validation layers");
            }
            vulkan.debug_messenger_create_info = get_vulkan_debug_messenger_create_info();
        }

        VkApplicationInfo app_info{};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = vulkan.config.application_name.c_str();
        app_info.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        app_info.pEngineName = vulkan.config.engine_name.c_str();
        app_info.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo instance_create_info{};
        instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_create_info.pApplicationInfo = &app_info;
        instance_create_info.enabledExtensionCount = vulkan.extensions.size();
        instance_create_info.ppEnabledExtensionNames = vulkan.extensions.data();
#ifdef GM_PLATFORM_MACOS
        instance_create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
        if (vulkan.config.validation_layers_enabled) {
            instance_create_info.pNext = &vulkan.debug_messenger_create_info;
            instance_create_info.enabledLayerCount = vulkan.validation_layers.size();
            instance_create_info.ppEnabledLayerNames = vulkan.validation_layers.data();
        }

        return vkCreateInstance(&instance_create_info, GM_VK_ALLOCATOR, &vulkan.instance);
    }

    void destroy_vulkan_instance(const Vulkan& vulkan) {
        vkDestroyInstance(vulkan.instance, GM_VK_ALLOCATOR);
    }

    VkResult create_vulkan_debug_messenger(Vulkan& vulkan) {
        auto create_debug_messenger_fn = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vulkan.instance, "vkCreateDebugUtilsMessengerEXT");
        if (!create_debug_messenger_fn) {
            GM_THROW("Could not get Vulkan debug messenger create function");
        }
        return create_debug_messenger_fn(vulkan.instance, &vulkan.debug_messenger_create_info, GM_VK_ALLOCATOR, &vulkan.debug_messenger);
    }

    void destroy_vulkan_debug_messenger(const Vulkan& vulkan) {
        auto destroy_debug_messenger_fn = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vulkan.instance, "vkDestroyDebugUtilsMessengerEXT");
        if (!destroy_debug_messenger_fn) {
            GM_THROW("Could not get Vulkan debug messenger destroy function");
        }
        destroy_debug_messenger_fn(vulkan.instance, vulkan.debug_messenger, GM_VK_ALLOCATOR);
    }

    VkResult create_vulkan_surface(Vulkan& vulkan) {
        return glfwCreateWindowSurface(vulkan.instance, vulkan.config.window->glfw_window, GM_VK_ALLOCATOR, &vulkan.surface);
    }

    void destroy_vulkan_surface(const Vulkan& vulkan) {
        vkDestroySurfaceKHR(vulkan.instance, vulkan.surface, GM_VK_ALLOCATOR);
    }

    bool is_vulkan_supported() {
        return glfwVulkanSupported() == GLFW_TRUE;
    }

    std::vector<const char*> get_required_vulkan_extensions(const Vulkan& vulkan) {
        u32 glfw_extension_count = 0;
        const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        if (glfw_extension_count == 0) {
            GM_THROW("Could not find any required Vulkan extensions");
        }

        std::vector extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
        if (extensions.empty()) {
            GM_THROW("Could not get required Vulkan extensions");
        }

#ifdef GM_PLATFORM_MACOS
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif
        if (vulkan.config.validation_layers_enabled) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    std::vector<VkExtensionProperties> get_available_vulkan_extensions() {
        u32 extension_count = 0;
        const char* layer_name = nullptr;
        VkExtensionProperties* extension_properties = nullptr;
        vkEnumerateInstanceExtensionProperties(layer_name, &extension_count, extension_properties);
        if (extension_count == 0) {
            GM_THROW("Could not find any available Vulkan extensions");
        }

        std::vector<VkExtensionProperties> extensions(extension_count);
        vkEnumerateInstanceExtensionProperties(layer_name, &extension_count, extensions.data());
        if (extensions.empty()) {
            GM_THROW("Could not find any available Vulkan extensions");
        }

        return extensions;
    }

    bool has_vulkan_extensions(const std::vector<const char*>& extensions) {
        std::vector<VkExtensionProperties> available_extensions = get_available_vulkan_extensions();
        for (const char* extension: extensions) {
            bool extensionFound = false;
            for (const VkExtensionProperties& availableExtension: available_extensions) {
                if (strcmp(extension, availableExtension.extensionName) == 0) {
                    extensionFound = true;
                    break;
                }
            }
            if (!extensionFound) {
                std::cerr << "Could not find extension " << extension << std::endl;
                return false;
            }
        }
        return true;
    }

    std::vector<const char*> get_required_vulkan_validation_layers() {
        return { "VK_LAYER_KHRONOS_validation" };
    }

    std::vector<VkLayerProperties> get_available_vulkan_validation_layers() {
        u32 count;
        VkLayerProperties* layer_properties = nullptr;
        vkEnumerateInstanceLayerProperties(&count, layer_properties);
        std::vector<VkLayerProperties> layers(count);
        vkEnumerateInstanceLayerProperties(&count, layers.data());
        return layers;
    }

    bool has_vulkan_validation_layers(const std::vector<const char*>& validation_layers) {
        std::vector<VkLayerProperties> available_validation_layers = get_available_vulkan_validation_layers();
        for (const char* layer_name: validation_layers) {
            bool layer_found = false;
            for (const auto& availableLayer: available_validation_layers) {
                if (strcmp(layer_name, availableLayer.layerName) == 0) {
                    layer_found = true;
                    break;
                }
            }
            if (!layer_found) {
                std::cerr << "Could not find validation layer " << layer_name << std::endl;
                return false;
            }
        }
        return true;
    }

    VkDebugUtilsMessengerCreateInfoEXT get_vulkan_debug_messenger_create_info() {
            VkDebugUtilsMessengerCreateInfoEXT create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

            create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                         | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                         | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

            create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                     | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                     | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

            create_info.pfnUserCallback = on_vulkan_debug_message;
            return create_info;
        }

    VKAPI_ATTR VkBool32 VKAPI_CALL on_vulkan_debug_message(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void* user_data
    ) {
        if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            GM_LOG_ERROR(callback_data->pMessage);
        } else if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            GM_LOG_WARNING(callback_data->pMessage);
        } else if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
            GM_LOG_INFO(callback_data->pMessage);
        } else {
            GM_LOG_TRACE(callback_data->pMessage);
        }
        return VK_FALSE;
    }

}
