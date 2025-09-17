#include "vulkan.h"

namespace dd {
    void initialize_vulkan(vulkan& vulkan) {
        DD_ASSERT(is_vulkan_supported(), "Vulkan is not supported");

        std::vector<const char*> required_extensions = get_required_extensions();
        DD_ASSERT(
            std::count(required_extensions.begin(), required_extensions.end(), "VK_KHR_surface") > 0,
            "Could not find Vulkan surface extension"
        );

#ifdef DD_PLATFORM_MACOS
        required_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif
        if (vulkan.config.validation_layers_enabled) {
            required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        DD_ASSERT(has_extensions(required_extensions), "System does not have required Vulkan extensions");

        std::vector<const char*> validation_layers;
        VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info{};
        if (vulkan.config.validation_layers_enabled) {
            validation_layers.push_back("VK_LAYER_KHRONOS_validation");
            DD_ASSERT(has_validation_layers(validation_layers), "System does not have required Vulkan validation layers");
            debug_messenger_create_info = get_debug_messenger_create_info();
        }

        create_vulkan_instance(vulkan, required_extensions, validation_layers, debug_messenger_create_info);
        if (vulkan.config.validation_layers_enabled) {
            create_debug_messenger(vulkan, debug_messenger_create_info);
        }
        create_surface(vulkan);

        pick_physical_device(vulkan);
        create_device(vulkan);
    }

    void terminate_vulkan(vulkan& vulkan) {
        destroy_device(vulkan);
        destroy_surface(vulkan);
        if (vulkan.config.validation_layers_enabled) {
            destroy_debug_messenger(vulkan);
        }
        destroy_vulkan_instance(vulkan);
    }

    void create_vulkan_instance(
        vulkan& vulkan,
        const std::vector<const char*>& required_extensions,
        const std::vector<const char*>& validation_layers,
        const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info
    ) {
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
        instance_create_info.enabledExtensionCount = required_extensions.size();
        instance_create_info.ppEnabledExtensionNames = required_extensions.data();
#ifdef DD_PLATFORM_MACOS
        instance_create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
        if (vulkan.config.validation_layers_enabled) {
            instance_create_info.pNext = &debug_messenger_create_info;
            instance_create_info.enabledLayerCount = validation_layers.size();
            instance_create_info.ppEnabledLayerNames = validation_layers.data();
        }

        DD_ASSERT_VK_SUCCESS(
            vkCreateInstance(&instance_create_info, DD_VK_ALLOCATOR, &vulkan.instance),
            "Could not create Vulkan instance"
        );
    }

    void destroy_vulkan_instance(vulkan& vulkan) {
        vkDestroyInstance(vulkan.instance, DD_VK_ALLOCATOR);
    }

    void create_debug_messenger(vulkan& vulkan, const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info) {
        auto create_debug_messenger = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vulkan.instance, "vkCreateDebugUtilsMessengerEXT");
        DD_ASSERT(create_debug_messenger != nullptr, "Could not get Vulkan debug messenger function");
        DD_ASSERT_VK_SUCCESS(
            create_debug_messenger(vulkan.instance, &debug_messenger_create_info, DD_VK_ALLOCATOR, &vulkan.debug_messenger),
            "Could not create debug messenger"
        );
    }

    void destroy_debug_messenger(vulkan& vulkan) {
        auto destroy_debug_messenger = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vulkan.instance, "vkDestroyDebugUtilsMessengerEXT");
        if (destroy_debug_messenger == nullptr) {
            return;
        }
        destroy_debug_messenger(vulkan.instance, vulkan.debug_messenger, DD_VK_ALLOCATOR);
    }

    void create_surface(vulkan& vulkan) {
        DD_ASSERT_VK_SUCCESS(
            glfwCreateWindowSurface(vulkan.instance, vulkan.config.window->glfw_window, DD_VK_ALLOCATOR, &vulkan.surface),
            "Could not create GLFW window surface"
        );
    }

    void destroy_surface(vulkan& vulkan) {
        vkDestroySurfaceKHR(vulkan.instance, vulkan.surface, DD_VK_ALLOCATOR);
    }

    bool is_vulkan_supported() {
        return glfwVulkanSupported() == GLFW_TRUE;
    }

    std::vector<const char*> get_required_extensions() {
        uint32_t glfw_extension_count = 0;
        const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        DD_ASSERT(glfw_extensions != nullptr, "Could not get required Vulkan extensions");
        std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
        DD_ASSERT(!extensions.empty(), "List of required Vulkan extensions was empty");
        return extensions;
    }

    std::vector<VkExtensionProperties> get_available_extensions() {
        uint32_t extension_count = 0;
        const char* layer_name = nullptr;
        VkExtensionProperties* extension_properties = nullptr;
        vkEnumerateInstanceExtensionProperties(layer_name, &extension_count, extension_properties);
        DD_ASSERT(extension_count > 0, "Could not find any available Vulkan extensions");
        std::vector<VkExtensionProperties> extensions(extension_count);
        vkEnumerateInstanceExtensionProperties(layer_name, &extension_count, extensions.data());
        DD_ASSERT(!extensions.empty(), "Could not find any available Vulkan extensions");
        return extensions;
    }

    bool has_extensions(const std::vector<const char*>& extensions) {
        std::vector<VkExtensionProperties> available_extensions = get_available_extensions();
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

    std::vector<VkLayerProperties> get_available_validation_layers() {
        uint32_t count;
        VkLayerProperties* layer_properties = nullptr;
        vkEnumerateInstanceLayerProperties(&count, layer_properties);
        std::vector<VkLayerProperties> layers(count);
        vkEnumerateInstanceLayerProperties(&count, layers.data());
        return layers;
    }

    bool has_validation_layers(const std::vector<const char*>& validation_layers) {
        std::vector<VkLayerProperties> available_validation_layers = get_available_validation_layers();
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

    VkDebugUtilsMessengerCreateInfoEXT get_debug_messenger_create_info() {
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
                DD_LOG_ERROR(callback_data->pMessage);
            } else if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
                DD_LOG_WARN(callback_data->pMessage);
            } else if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
                DD_LOG_INFO(callback_data->pMessage);
            } else {
                DD_LOG_TRACE(callback_data->pMessage);
            }
            return VK_FALSE;
        }

}
