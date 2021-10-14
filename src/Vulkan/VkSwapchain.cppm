/**
 * @file VkSwapchain.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides abstraction above Vulkan swapchain
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.Swapchain;

export import App.NativeWrapper;

import Vulkan;
import GLFW;

import <vector>;

import Vk.Getter;
import Vk.Checker;
import App.Window;
import Vk.LogicalDevice;
import Vk.WindowSurface;
import Vk.PhysicalDevice;

/**
 * @class Swapchain
 * @brief class provides api for setup 
 * and manege native swapchain
 * 
 */
export class Swapchain:
    public NativeWrapper<VkSwapchainKHR, Swapchain> {
private:
    VkExtent2D                      _extent;
    VkSurfaceFormatKHR              _format;
    LogicalDevice::const_pointer    _ld;

public:
    /**
     * @brief capture native swapchain handle
     * 
     */
    Swapchain(PhysicalDevice::const_pointer device, 
              LogicalDevice::const_pointer ld, 
              WindowSurface::const_pointer surface, 
              Window::const_pointer window);
    
    /**
     * @brief release handle
     * 
     */
    ~Swapchain();

    /**
     * @brief get swapchain setup infos
     * 
     */
    VkExtent2D getExtent() const;
    VkSurfaceFormatKHR getFormat() const;

private:
    /**
     * @brief helper methods for capturing swapchain
     * 
     */
    void setup(PhysicalDevice::const_pointer device,  
               WindowSurface::const_pointer surface, 
               Window::const_pointer window);

    void setupFormat(PhysicalDevice::const_pointer device, WindowSurface::const_pointer surface);
    VkPresentModeKHR getMode(PhysicalDevice::const_pointer device, WindowSurface::const_pointer surface); 

    void setupExtent(Window::const_pointer window, VkSurfaceCapabilitiesKHR cap);
}; // Swapchain


/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
Swapchain::Swapchain(PhysicalDevice::const_pointer device, 
                     LogicalDevice::const_pointer ld, 
                     WindowSurface::const_pointer surface, 
                     Window::const_pointer window) : _ld(ld) {
    setup(device, surface, window);
}

Swapchain::~Swapchain() {
    vkDestroySwapchainKHR(_ld->get(), _native, nullptr);
}

void Swapchain::setup(PhysicalDevice::const_pointer device, WindowSurface::const_pointer surface, Window::const_pointer window) {
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->get(), surface->get(), &capabilities);

    setupExtent(window, capabilities);
    setupFormat(device, surface);

    uint32_t imageCount;

    if (capabilities.maxImageCount > 0) {
        imageCount = capabilities.maxImageCount;
    }
    else {
        imageCount = capabilities.minImageCount + 5;

    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface->get();
    createInfo.minImageCount = capabilities.maxImageCount > 0? capabilities.maxImageCount: capabilities.minImageCount + 5;
    createInfo.imageFormat = _format.format;
    createInfo.imageColorSpace = _format.colorSpace;
    createInfo.imageExtent = _extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = getMode(device, surface);
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    VkCreate<vkCreateSwapchainKHR>(_ld->get(), &createInfo, nullptr, &_native);
}


void Swapchain::setupFormat(PhysicalDevice::const_pointer device, WindowSurface::const_pointer surface) {
    VkSurfaceFormatKHR              format;
    std::vector<VkSurfaceFormatKHR> formats;
    
    VkGet<vkGetPhysicalDeviceSurfaceFormatsKHR>(formats, device->get(), surface->get());

    format = formats.at(0);

    for (const auto& availableFormat : formats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB 
        && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            format = availableFormat;
        }
    }

    _format = format;
}

VkPresentModeKHR Swapchain::getMode(PhysicalDevice::const_pointer device, WindowSurface::const_pointer surface) {
    std::vector<VkPresentModeKHR> presentModes;
    VkPresentModeKHR              mode;

    VkGet<vkGetPhysicalDeviceSurfacePresentModesKHR>(presentModes, device->get(), surface->get());    
    if(std::find(presentModes.begin(), presentModes.end(), VK_PRESENT_MODE_MAILBOX_KHR) != presentModes.end()) {
        mode = VK_PRESENT_MODE_MAILBOX_KHR;
    } else if(std::find(presentModes.begin(), presentModes.end(), VK_PRESENT_MODE_FIFO_RELAXED_KHR) != presentModes.end()) {
        mode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
    } else if(std::find(presentModes.begin(), presentModes.end(), VK_PRESENT_MODE_FIFO_KHR) != presentModes.end()) {
        mode = VK_PRESENT_MODE_FIFO_KHR;
    }

    return mode;
}


void Swapchain::setupExtent(Window::const_pointer window, VkSurfaceCapabilitiesKHR cap) {
    if (cap.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        _extent = cap.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window->get(), &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::max(cap.minImageExtent.width, std::min(cap.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(cap.minImageExtent.height, std::min(cap.maxImageExtent.height, actualExtent.height));

        _extent = actualExtent;
    }
}

VkExtent2D Swapchain::getExtent() const {
    return _extent;
}


VkSurfaceFormatKHR Swapchain::getFormat() const {
    return _format;
}