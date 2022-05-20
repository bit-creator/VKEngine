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

import <stdexcept>;
import <memory>;

export struct Layout: 
    public vk::NativeWrapper<VkPipelineLayout> {
    Layout(const LogicalDevice& device);
};


Layout::Layout(const LogicalDevice& device)
    : Internal([&device](const value_type& l)
        { vkDestroyPipelineLayout(device, l, nullptr); }) {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; 
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &_native) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}