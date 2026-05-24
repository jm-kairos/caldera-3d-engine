#pragma once

#include "defines.h"
#include "core\asserts.h"

#include "containers/vector.h"

#include <vulkan/vulkan.h>

#define IBX_VK_EVAL(_result) { IBX_ASSERT(_result == VK_SUCCESS) }

struct VulkanSwapchainSupportInfo{
    VkSurfaceCapabilitiesKHR capabilities;
    Vector(VkSurfaceFormatKHR) formats;
    Vector(VkPresentModeKHR) present_modes;
};

struct VulkanDevice{
    VkPhysicalDevice chosen_gpu_device;
    VkDevice logical_device;
    VulkanSwapchainSupportInfo swapchain_support_info;
    i32 graphics_queue_index;
    i32 present_queue_index;
    i32 transfer_queue_index;
    i32 compute_queue_index;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memory;
};

struct VulkanContext{
    VkInstance instance;
    VkAllocationCallbacks* allocator;
    VkSurfaceKHR surface;
    VulkanDevice device;
};