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

export class Framebuffer:
    public vk::NativeWrapper<VkFramebuffer> {
public:
    Framebuffer(VkExtent2D ext, const RenderPass& pass, const ImageView& view, LogicalDevice device);
};


Framebuffer::Framebuffer(VkExtent2D ext, const RenderPass& pass, const ImageView& view, LogicalDevice device)
    : Internal([&](value_type fb){ vkDestroyFramebuffer(device, fb, nullptr); }) 
    {
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = pass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &(view.native());
    framebufferInfo.width = ext.width;
    framebufferInfo.height =ext.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &_native) != VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
    }
}
