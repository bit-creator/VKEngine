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

export import App.NativeWrapper;

import Vulkan;
import Vk.LogicalDevice;
import Vk.Checker;

import <stdexcept>;
import <memory>;

export class Layout: 
    public NativeWrapper<VkPipelineLayout, Layout> {
private:
    // VkPipelineLayout                        _native;
    const LogicalDevice&                    _device;

public:
    Layout(const LogicalDevice& device);
    ~Layout();

    // operator VkPipelineLayout() const;
    // operator VkPipelineLayout();

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

    // VkCreate<vkCreatePipelineLayout>(device, &pipelineLayoutInfo, nullptr, &_native);

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &_native) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

Layout::~Layout() {
    vkDestroyPipelineLayout(_device, _native, nullptr);
}


// Layout::operator VkPipelineLayout() const {
//     return _native;
// }

// Layout::operator VkPipelineLayout() {
//     return _native;
// }

// VkPipelineLayoutCreateInfo
// Layout::getState() {
// }