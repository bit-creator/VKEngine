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

import Vulkan;
import GLFW;

import <limits>;
import <vector>;
import <iostream>;
import <algorithm>;

import Vk.Getter;
import App.Window;
import Vk.LogicalDevice;
import Vk.WindowSurface;
import Vk.PhysicalDevice;

export class Swapchain {
    VkSwapchainKHR                  _swapChain;
    VkExtent2D                      _extent;
    VkSurfaceFormatKHR              _format;
    const LogicalDevice&            _ld;

public:
    Swapchain(const PhysicalDevice& device, const LogicalDevice& ld, const WindowSurface& surface, const Window& window);
    ~Swapchain();

    VkExtent2D getExtent() const;
    VkSurfaceFormatKHR getFormat() const;

    Swapchain(const Swapchain&) =delete;
    Swapchain(Swapchain&&) =delete;

    operator VkSwapchainKHR();
    operator VkSwapchainKHR() const;

private:
    void setup(const PhysicalDevice& device, const WindowSurface& surface, const Window& window);
    void setupFormat(const PhysicalDevice& device, const WindowSurface& surface);
    VkPresentModeKHR   getMode(const PhysicalDevice& device, const WindowSurface& surface); 

    void setupExtent(const Window& window, VkSurfaceCapabilitiesKHR cap);
};


Swapchain::Swapchain(const PhysicalDevice& device, const LogicalDevice& ld, const WindowSurface& surface, const Window& window) : _ld(ld) {
    VkBool32 presentSupport = false;
    uint32_t queueIndex =0;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, queueIndex, surface, &presentSupport);
    if(!presentSupport) throw std::runtime_error("present queue not supported");
    setup(device, surface, window);
}

Swapchain::~Swapchain() {
    vkDestroySwapchainKHR(_ld, _swapChain, nullptr);
}

void Swapchain::setup(const PhysicalDevice& device, const WindowSurface& surface, const Window& window) {
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);

    setupExtent(window, capabilities);
    setupFormat(device, surface);

    uint32_t imageCount;

    if (capabilities.maxImageCount > 0) {
        imageCount = capabilities.maxImageCount;
    }
    else {
        imageCount = capabilities.minImageCount + 5;

    }

    // surface.getPresentQueue(device);


    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
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

    if (vkCreateSwapchainKHR(_ld, &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }
}


void Swapchain::setupFormat(const PhysicalDevice& device, const WindowSurface& surface) {
    VkSurfaceFormatKHR              format;
    std::vector<VkSurfaceFormatKHR> formats;
    
    VkGet<vkGetPhysicalDeviceSurfaceFormatsKHR>(formats, (VkPhysicalDevice)device, (VkSurfaceKHR)surface);

    format = formats.at(0);

    for (const auto& availableFormat : formats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            format = availableFormat;
        }
    }

    _format = format;
}

VkPresentModeKHR Swapchain::getMode(const PhysicalDevice& device, const WindowSurface& surface) {
    std::vector<VkPresentModeKHR> presentModes;
    VkPresentModeKHR              mode;

    VkGet<vkGetPhysicalDeviceSurfacePresentModesKHR>(presentModes, (VkPhysicalDevice)device, (VkSurfaceKHR)surface);    
    if(std::find(presentModes.begin(), presentModes.end(), VK_PRESENT_MODE_MAILBOX_KHR) != presentModes.end()) {
        mode = VK_PRESENT_MODE_MAILBOX_KHR;
    } else if(std::find(presentModes.begin(), presentModes.end(), VK_PRESENT_MODE_FIFO_RELAXED_KHR) != presentModes.end()) {
        mode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
    } else if(std::find(presentModes.begin(), presentModes.end(), VK_PRESENT_MODE_FIFO_KHR) != presentModes.end()) {
        mode = VK_PRESENT_MODE_FIFO_KHR;
    }

    return mode;
}


void Swapchain::setupExtent(const Window& window, VkSurfaceCapabilitiesKHR cap) {
    if (cap.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        _extent = cap.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

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

Swapchain::operator VkSwapchainKHR() {
    return _swapChain;
}

Swapchain::operator VkSwapchainKHR() const {
    return _swapChain;
}