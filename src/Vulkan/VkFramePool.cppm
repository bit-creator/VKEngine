/**
 * @file VkFrame.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on frames collection
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.FramePool;

import Vulkan;

import <iostream>;

import Vk.Frame;
import Vk.Swapchain;
import Vk.LogicalDevice;
import Vk.Getter;
import Vk.RenderPass;

import <vector>;

export class FramePool {
private:
    std::vector<Frame>                  _frames;

public:
    FramePool(const Swapchain& swapchain, const LogicalDevice& device, const RenderPass& pass);
    ~FramePool();
};


FramePool::FramePool(const Swapchain& swapchain, const LogicalDevice& device, const RenderPass& pass) {
    std::vector<VkImage>        images;

    VkGet<vkGetSwapchainImagesKHR>(images, (VkDevice)device, (VkSwapchainKHR)swapchain);

    _frames.reserve(images.size());

    for(auto img: images) {
        _frames.emplace_back(img, swapchain, device, pass);
    }
}

FramePool::~FramePool() {
}