/**
 * @file VkLayout.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on layouts
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.Layout;

export import App.NativeWrapper;

import Vulkan;
import Vk.LogicalDevice;
import Vk.Checker;
import Math.Matrix4f;
import Vk.DescriptorPool;

import <stdexcept>;
import <memory>;

export struct Layout: 
    public vk::NativeWrapper<VkPipelineLayout> {
    Layout(const LogicalDevice& device, DescriptorSetLayout layout);
};

export struct Transform {
    mathon::Matrix4f        _mvp;
};

Layout::Layout(const LogicalDevice& device, DescriptorSetLayout layout)
    : Internal([&device](const value_type& l)
        { vkDestroyPipelineLayout(device, l, nullptr); }) {
    VkPushConstantRange push;
    push.offset =0;
    push.size = sizeof(Transform);
    push.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &layout.native();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &push;

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &_native) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}