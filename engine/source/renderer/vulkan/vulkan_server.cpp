#include "vulkan_server.h"

#include "renderer/renderer_types.h"
#include "vulkan_platform.h"

#include "containers/vector.h" 
#include "containers/string.h"

#include "platform/platform.h"

static VulkanContext context = {};

b8 vulkan_renderer_server_initialize(RendererServer *renderer_server, const char *app_name, platform_state *plat_stat)
{  
    // TODO: custom allocator
    context.allocator = 0;

    VkApplicationInfo app_info;
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = 0;
    app_info.pApplicationName = app_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);;
    app_info.pEngineName = "Erg Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo create_instance_info; 
    create_instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_instance_info.pNext = 0;
    create_instance_info.flags = 0;
    create_instance_info.pApplicationInfo = &app_info;

    // Process validation Layers in here.

    create_instance_info.enabledLayerCount = 0;
    create_instance_info.ppEnabledLayerNames = 0;

    Vector(const char*) required_extensions =  {};
    required_extensions.reserve(32);
    required_extensions.push_back( VK_KHR_SURFACE_EXTENSION_NAME ); // Generic surface extension.
    // Get platform specific extensions.
    platform_get_required_extension_names(required_extensions);

#if defined(ERG_DEBUG)
    required_extensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME ); // Debug utilities.

    ERG_LOG_DEBUG("Required extensions:");
    const size_t length = required_extensions.size();

    for (size_t i = 0; i < length; i++)
        ERG_LOG_DEBUG(required_extensions[i])

#endif

    create_instance_info.enabledExtensionCount = required_extensions.size();
    create_instance_info.ppEnabledExtensionNames = required_extensions.data();

    VkResult result = vkCreateInstance(&create_instance_info, context.allocator, &context.instance);
    ERG_VULKAN_EVALUATE_ERROR(result)

    ERG_LOG_INFO("Vulkan renderer initialized successfully.")
    return TRUE;
}

void vulkan_renderer_server_terminate(RendererServer *renderer_server)
{
}

void vulkan_renderer_server_resized(RendererServer *renderer_server, u16 width, u16 height)
{
}

b8 vulkan_renderer_server_begin_frame(RendererServer *renderer_server, real dt)
{
    return b8();
}

b8 vulkan_renderer_server_end_frame(RendererServer *renderer_server, real dt)
{
    return b8();
}
