/**
 * @file VkImageView.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on image presentation
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.ImageView;

export import App.NativeWrapper;

import Vulkan;

import Vk.LogicalDevice;
import Vk.Swapchain;

import <iostream>;

export struct ImageView:
        public vk::NativeWrapper<VkImageView> {
    ImageView(VkImage img, VkFormat format, LogicalDevice device,
            VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT);
};


ImageView::ImageView(VkImage img, VkFormat format,
                    LogicalDevice device, VkImageAspectFlags aspect):
        Internal([&](value_type img){ vkDestroyImageView(device, img, nullptr); }) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = img;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = format;
    
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    createInfo.subresourceRange.aspectMask = aspect;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device, &createInfo, nullptr, &_native) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image views!");
    }
}
