/**
 * @file Vkframebuffer.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on framebuffers
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.Framebuffer;

export import App.NativeWrapper;

import Vulkan;

import Vk.LogicalDevice;
import Vk.ImageView;
import Vk.RenderPass;
import <vector>;
import <iostream>;

export struct Framebuffer:
        public vk::NativeWrapper<VkFramebuffer> {
    Framebuffer(VkExtent2D ext, const RenderPass& pass,
            std::vector<VkImageView> views, LogicalDevice device);
};


Framebuffer::Framebuffer(VkExtent2D ext, const RenderPass& pass,
            std::vector<VkImageView> views, LogicalDevice device)
        : Internal([&](value_type fb){ vkDestroyFramebuffer(device, fb, nullptr); }) {

    std::cout << views[0] << std::endl;
    std::cout << views[1] << std::endl;

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = pass;
    framebufferInfo.attachmentCount = (uint32_t)views.size();
    framebufferInfo.pAttachments = views.data();
    framebufferInfo.width = ext.width;
    framebufferInfo.height =ext.height;
    framebufferInfo.layers = 1;

    // std::cout << "fb(_native1 == nullptr) = " << (views[0].native() == VK_NULL_HANDLE) << std::endl;
    // std::cout << "fb(_native2 == nullptr) = " << (views[0].native() == VK_NULL_HANDLE) << std::endl;

    if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &_native) != VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
    }
}
