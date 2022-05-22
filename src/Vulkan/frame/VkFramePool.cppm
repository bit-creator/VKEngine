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

export import App.NativeWrapper;

import Vulkan;

import <iostream>;

import Vk.ImageView;
import Vk.Framebuffer;
import Vk.CommandBuffer;
import Vk.Frame;
import Vk.Swapchain;
import Vk.LogicalDevice;
import Vk.Getter;
import Vk.Pipeline;
import Vk.Queue;
import Vk.QueuePool;
import Vk.Semaphore;
import Vk.CommandPool;

import <vector>;

export class FramePool: public DrawCmdPool {
private:
    std::vector<Frame>                    _frames;

public:
    FramePool(Swapchain swapchain, LogicalDevice device, Pipeline& pipe);

    const Frame& operator [](int i);

    size_t size();
};


FramePool::FramePool( Swapchain swapchain, LogicalDevice device, Pipeline& pipe): 
   DrawCmdPool(device) {
    std::vector<VkImage>                        images;
   
    VkGet<vkGetSwapchainImagesKHR>(images, device, swapchain);

    _frames.reserve(images.size());
   
    for(uint32_t i =0; i < images.size(); ++i) {
        _frames.emplace_back(images[i], _native, swapchain, device, pipe);
    }
}

const Frame& FramePool::operator [](int i) {
    return _frames[i];
}

size_t FramePool::size() {
    return _frames.size();
}