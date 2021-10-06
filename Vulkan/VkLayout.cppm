/**
 * @file VkColorBlender.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on layouts
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.Layout;

import Vk;
import Vk.LogicalDevice;

import <stdexcept>;

export class Layout {
private:
    VkPipelineLayout                        _layout;
    const LogicalDevice&                    _device;

public:
    Layout(const LogicalDevice& device);
    ~Layout();

    // VkPipelineLayoutCreateInfo
    // getState();
};


Layout::Layout(const LogicalDevice& device): _device(device) {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; 
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &_layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

Layout::~Layout() {
    vkDestroyPipelineLayout(_device, _layout, nullptr);
}

// VkPipelineLayoutCreateInfo
// Layout::getState() {
// }