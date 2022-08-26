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

export import App.NativeWrapper;
import Vk.ColorAttachment;
import Vk.LogicalDevice;
import Vk.Swapchain;
import Vk.PhysicalDevice;

import <stdexcept>;
import <vector>;
import <array>;

export struct RenderPass:
        public vk::NativeWrapper<VkRenderPass> {
    RenderPass(LogicalDevice device, VkFormat format);
};


RenderPass::RenderPass(LogicalDevice device, VkFormat format) 
        : Internal([&](value_type p){ vkDestroyRenderPass(device, p, nullptr); }) {
    VkFormat dformat = VK_FORMAT_D32_SFLOAT_S8_UINT;

    // std::vector<VkFormat> candidates = {
    //     VK_FORMAT_D32_SFLOAT_S8_UINT,
    //     VK_FORMAT_D24_UNORM_S8_UINT,
    //     VK_FORMAT_D32_SFLOAT
    // };

    // VkImageTiling        tiling     = VK_IMAGE_TILING_OPTIMAL;
    // VkFormatFeatureFlags features   = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

    // for (VkFormat format : candidates) {
    //     VkFormatProperties props;
    //     vkGetPhysicalDeviceFormatProperties(phys, format, &props);
    //     if (tiling == VK_IMAGE_TILING_LINEAR &&
    //             (props.linearTilingFeatures & features) == features) {
    //         dformat = format; break;
    //     } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
    //             (props.optimalTilingFeatures & features) == features) {
    //         dformat = format; break;
    //     }
    // }

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = dformat;
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    
    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    
    std::vector<VkAttachmentDescription> attachments = {colorAttachment, depthAttachment};
    
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcAccessMask = 0;

    dependency.srcStageMask = 
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    
    dependency.dstStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    
    dependency.dstAccessMask =
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = (uint32_t)attachments.size();
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;
    
    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &_native) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}