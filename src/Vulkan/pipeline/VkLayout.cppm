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

import <stdexcept>;
import <memory>;

export struct Layout: 
    public vk::NativeWrapper<VkPipelineLayout> {
    Layout(const LogicalDevice& device);
};

export struct Transform {
    mathon::Matrix4f        _mvp;
};

Layout::Layout(const LogicalDevice& device)
    : Internal([&device](const value_type& l)
        { vkDestroyPipelineLayout(device, l, nullptr); }) {
}