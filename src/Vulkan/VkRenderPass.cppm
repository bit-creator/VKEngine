/**
 * @file VkRenderPass.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on render passes
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.RenderPass;

import Vulkan;

import Vk.ColorAttachment;
import Vk.LogicalDevice;
import Vk.Swapchain;

import <stdexcept>;

export struct RenderSubPass {
    RenderSubPass() {
    }

    ~RenderSubPass() {
    }

    VkSubpassDescription
    getDescription(size_t ind) {
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = ind;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        return subpass;
    }
};

export class RenderPass {
private:           
    VkRenderPass                            _pass;
    const LogicalDevice&                    _device;
    // ColorAttachment                         _attach;


public:
    RenderPass(const LogicalDevice& device, const Swapchain& swapchain);

    ~RenderPass();
};


RenderPass::RenderPass(const LogicalDevice& device, const Swapchain& swapchain) 
    : _device(device)
    // , _attach(swapchain) 
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = swapchain.getFormat().format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &_pass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }

        
    // auto subpass = RenderSubPass().getDescription(0);
    // auto colAttach = _attach.getDescription();
    // VkRenderPassCreateInfo renderPassInfo{};
    // renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    // renderPassInfo.attachmentCount = 1;
    // renderPassInfo.pAttachments = &colAttach;
    // renderPassInfo.subpassCount = 1;
    // renderPassInfo.pSubpasses = &subpass;

    // if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &_pass) != VK_SUCCESS) {
    //     throw std::runtime_error("failed to create render pass!");
    // }
}

RenderPass::~RenderPass() {
    vkDestroyRenderPass(_device, _pass, nullptr);
}