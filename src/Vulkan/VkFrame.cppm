/**
 * @file VkFrame.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on frame build process
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.Frame;

import <vector>;
import <iostream>;

import Vulkan;
import Vk.Swapchain;
import Vk.LogicalDevice;
import Vk.RenderPass;

export class Frame {
private:
    const LogicalDevice*            _device;
    VkImage                         _image;
    VkImageView                     _view;
    VkFramebuffer                   _buffer;
    // VkCommandBuffer ...

public:
    Frame(VkImage img, const Swapchain& swapchain, const LogicalDevice& device, const RenderPass& pass);
    ~Frame();

    Frame(const Frame&) =delete;
    Frame& operator =(const Frame&) =delete;

    Frame(Frame&&) =default;
    Frame& operator =(Frame&&) =default;
};

Frame::Frame(VkImage img, const Swapchain& swapchain, const LogicalDevice& device, const RenderPass& pass): 
        _device(&device), _image(img) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = img;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swapchain.getFormat().format;
    
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    // std::cout << createInfo.format << std::endl;

    if (vkCreateImageView(device, &createInfo, nullptr, &_view) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image views!");
    }

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = pass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &_view;
    framebufferInfo.width = swapchain.getExtent().width;
    framebufferInfo.height = swapchain.getExtent().height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
    }
}

Frame::~Frame() {
    vkDestroyFramebuffer(*_device, _buffer, nullptr);
    vkDestroyImageView(*_device, _view, nullptr);
}