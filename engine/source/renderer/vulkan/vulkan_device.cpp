#include "vulkan_device.h"

#include "core/logger.h"
#include "containers/vector.h"

struct VulkanPhysicalDeviceRequirements
{
    b8 graphics;
    b8 present;
    b8 compute;
    b8 transfer;
    Vector(const char*) device_extension_names;
    b8 sampler_anisotropy;
    b8 discrete_gpu;
};

struct VulkanPhysicalDeviceQueueFamilyInfo
{
    u32 graphics_family_index;
    u32 present_family_index;
    u32 compute_family_index;
    u32 transfer_family_index;
};

void vulkan_physical_device_query_swapchain_support(
    VkPhysicalDevice device, 
    VkSurfaceKHR surface, 
    VulkanSwapchainSupportInfo* out_swapchain_support);

b8 vulkan_check_physical_device_suitability(
    VkPhysicalDevice device,
    VkSurfaceKHR surface,
    const VkPhysicalDeviceProperties* properties,
    const VkPhysicalDeviceFeatures* features,
    const VulkanPhysicalDeviceRequirements* requirements,
    VulkanPhysicalDeviceQueueFamilyInfo* out_queue_family_info,
    VulkanSwapchainSupportInfo* out_swapchain_support);

b8 vulkan_select_physical_device(VulkanContext* context){

    u32 physical_devices_count = 0;
    IBX_VK_EVAL(vkEnumeratePhysicalDevices(context->instance, &physical_devices_count, 0));

    Vector(VkPhysicalDevice) physical_devices;
    physical_devices.reserve(physical_devices_count);

    IBX_VK_EVAL(vkEnumeratePhysicalDevices(context->instance, &physical_devices_count, physical_devices.data()));

    IBX_LOG_INFO("Searching for a suitable Physical Device:");
    b8 found = FALSE;
    for (u32 i = 0; i < physical_devices_count; ++i)
    {
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;
        VkPhysicalDeviceMemoryProperties memory;
        vkGetPhysicalDeviceProperties(physical_devices[i], &properties);
        vkGetPhysicalDeviceFeatures(physical_devices[i], &features);
        vkGetPhysicalDeviceMemoryProperties(physical_devices[i], &memory);

        VulkanPhysicalDeviceRequirements requirements;
        requirements.compute = TRUE;
        requirements.device_extension_names = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        requirements.discrete_gpu = TRUE;
        requirements.graphics = TRUE;
        requirements.present = TRUE;
        requirements.sampler_anisotropy = TRUE;
        requirements.transfer = TRUE;

        VulkanPhysicalDeviceQueueFamilyInfo queue_family_info = {};

        b8 is_suitable = vulkan_check_physical_device_suitability(
            physical_devices[i],
            context->surface,
            &properties,
            &features,
            &requirements,
            &queue_family_info,
            &context->device.swapchain_support_info);

        if (is_suitable)
        {
            IBX_LOG_INFO("Selected physical device: %s", properties.deviceName);
            switch (properties.deviceType)
            {
            case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                IBX_LOG_INFO("gpu type is unknown.")
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                IBX_LOG_INFO("gpu type is integrated.")
                break;
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                IBX_LOG_INFO("gpu type is discrete.")
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                IBX_LOG_INFO("gpu type is virtual.")
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                IBX_LOG_INFO("gpu type is cpu.")
                break;
            default:
                break; 
            }
            IBX_LOG_INFO("gpu driver version: %d.%d.%d",
            VK_VERSION_MAJOR(properties.driverVersion),
            VK_VERSION_MINOR(properties.driverVersion),
            VK_VERSION_PATCH(properties.driverVersion))
            // Vulkan API version.
            IBX_LOG_INFO("vulkan api version: %d.%d.%d",
            VK_VERSION_MAJOR(properties.apiVersion),
            VK_VERSION_MINOR(properties.apiVersion),
            VK_VERSION_PATCH(properties.apiVersion))

            // TODO: Extract memory information from memory object. See Episode 015.

            found = TRUE;

            context->device.chosen_gpu_device = physical_devices[i];
            context->device.graphics_queue_index = queue_family_info.graphics_family_index;
            context->device.present_queue_index = queue_family_info.present_family_index;
            context->device.transfer_queue_index = queue_family_info.transfer_family_index;
            context->device.compute_queue_index = queue_family_info.compute_family_index;
            context->device.properties = properties;
            context->device.features = features;
            context->device.memory = memory;

            break;
        }
    }

    if (!found)
    {
        IBX_LOG_FATAL("No suitable Physical devices found.");
        return FALSE;
    }

    return TRUE;
}

b8 vulkan_device_create(VulkanContext* context){
    if (!vulkan_select_physical_device(context)){
        return FALSE;
    }

    IBX_LOG_INFO("Creating logical device.")

    return TRUE;
}

void vulkan_device_release(VulkanContext* context){
    IBX_LOG_INFO("Releasing physical device resources.")
    context->device.chosen_gpu_device = 0;
    context->device.graphics_queue_index = -1;
    context->device.present_queue_index = -1;
    context->device.transfer_queue_index = -1;
    context->device.compute_queue_index = -1;
    vkDestroyDevice(context->device.logical_device, context->allocator);
}

b8 vulkan_check_physical_device_suitability(
    VkPhysicalDevice device,
    VkSurfaceKHR surface,
    const VkPhysicalDeviceProperties* properties,
    const VkPhysicalDeviceFeatures* features,
    const VulkanPhysicalDeviceRequirements* requirements,
    VulkanPhysicalDeviceQueueFamilyInfo* out_queue_family_info,
    VulkanSwapchainSupportInfo* out_swapchain_support)
{
    out_queue_family_info->compute_family_index = -1;
    out_queue_family_info->graphics_family_index = -1;
    out_queue_family_info->present_family_index = -1;
    out_queue_family_info->transfer_family_index = -1;

    if (requirements->discrete_gpu)
    {
        if (properties->deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            IBX_LOG_INFO("Physical device is not discrete. Skipping.")
            return FALSE;
        }
    }
    
    u32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, 0);

    Vector(VkQueueFamilyProperties) queue_families;
    queue_families.reserve(queue_family_count);

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    // Look at each queue and see what queue it supports
    IBX_LOG_INFO("graphics | present | compute | name")
    u8 min_transfer_score = 255;
    for (u32 i = 0; i < queue_family_count; ++i)
    {
        u8 current_transfer_score = 0;
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            out_queue_family_info->graphics_family_index = i;
            ++current_transfer_score;
        }

        if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            out_queue_family_info->compute_family_index = i;
            ++current_transfer_score;
        }

        if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
        {
            if (current_transfer_score <= min_transfer_score)
            {
                min_transfer_score = current_transfer_score;
                out_queue_family_info->transfer_family_index = i;
            }
        } 

        VkBool32 supports_present = VK_FALSE;
        IBX_VK_EVAL(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supports_present));
        if (supports_present){
            out_queue_family_info->present_family_index = i;
        }
        
    }    
    IBX_LOG_INFO("      %d |      %d |      %d |      %d | %s",
    out_queue_family_info->graphics_family_index != -1,
    out_queue_family_info->present_family_index != -1,
    out_queue_family_info->compute_family_index != -1,
    out_queue_family_info->transfer_family_index != -1,
    properties->deviceName)

    b8 g = (!requirements->graphics || (requirements->graphics && out_queue_family_info->graphics_family_index != -1));
    b8 p = (!requirements->present || (requirements->present && out_queue_family_info->present_family_index != -1));
    b8 c = (!requirements->compute || (requirements->compute && out_queue_family_info->compute_family_index != -1));
    b8 t = (!requirements->transfer || (requirements->transfer && out_queue_family_info->transfer_family_index != -1));

    if (g && p && c && t)
    {
        IBX_LOG_INFO("Physical device meets the requirements.")
        IBX_LOG_TRACE("Graphics Family Index: %i", out_queue_family_info->graphics_family_index)
        IBX_LOG_TRACE("Present Family Index: %i", out_queue_family_info->present_family_index)
        IBX_LOG_TRACE("Transfer Family Index: %i", out_queue_family_info->transfer_family_index)
        IBX_LOG_TRACE("Compute Family Index: %i", out_queue_family_info->compute_family_index)
    
        vulkan_physical_device_query_swapchain_support(device, surface, out_swapchain_support);

        if (out_swapchain_support->formats.size() < 1 || out_swapchain_support->present_modes.size() < 1)
        {
            IBX_LOG_INFO("Physical device does not support swapchain. Skipping.")
            return FALSE; 
        }

        if (requirements->device_extension_names.data())
        {
            u32 available_extensions_count = 0;
            IBX_VK_EVAL(vkEnumerateDeviceExtensionProperties(device, 0, &available_extensions_count, 0))

            Vector(VkExtensionProperties) available_extensions;
            available_extensions.reserve(available_extensions_count);

            IBX_VK_EVAL(vkEnumerateDeviceExtensionProperties(device, 0, &available_extensions_count, available_extensions.data()))
            
            // Verify availability of all required layers.
            size_t required_extensions_count = requirements->device_extension_names.size();
            for (size_t i = 0; i < required_extensions_count; ++i)
            {
                b8 found = FALSE;
                for (size_t j = 0; j < available_extensions_count; ++j)
                {
                    if (strcmp(requirements->device_extension_names[i], available_extensions[j].extensionName))
                    {
                        found = TRUE;
                        break;
                    }
                }
                if (!found)
                {
                    IBX_LOG_INFO("Required extension not found: %s, skipping device.", requirements->device_extension_names[i]);
                    return FALSE;
                }   
            }
        }

        if (requirements->sampler_anisotropy && !features->samplerAnisotropy)
        {
            IBX_LOG_INFO("Device does not support samplerAnisotropy, skipping.")
            return FALSE;
        }

        return TRUE;
    }
    
    return FALSE;
}

void vulkan_physical_device_query_swapchain_support(
    VkPhysicalDevice device, 
    VkSurfaceKHR surface, 
    VulkanSwapchainSupportInfo* out_swapchain_support)
{
    IBX_LOG_DEBUG("Querying for Swapchain Support.")

    IBX_VK_EVAL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &out_swapchain_support->capabilities))
    
    // Formats.
    u32 format_count = 0;
    IBX_VK_EVAL(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, 0));
    out_swapchain_support->formats.resize(format_count);
    IBX_VK_EVAL(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, out_swapchain_support->formats.data()));

    // Present modes.
    u32 present_modes_count = 0;
    IBX_VK_EVAL(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_modes_count, 0));
    out_swapchain_support->present_modes.resize(present_modes_count);
    IBX_VK_EVAL(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_modes_count, out_swapchain_support->present_modes.data()));
}
