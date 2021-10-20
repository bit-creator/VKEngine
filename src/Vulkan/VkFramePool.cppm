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
import Vk.Commandbuffer;
import Vk.Frame;
import Vk.Swapchain;
import Vk.LogicalDevice;
import Vk.Getter;
import Vk.Pipeline;
import Vk.Queue;
import Vk.QueuePool;
import Vk.Semaphore;

import <vector>;

export class FramePool:
    public NativeWrapper<VkCommandPool, FramePool> {
private:
    std::vector<Frame>                    _frames;
    LogicalDevice::const_pointer          _ld;
    const QueuePool&                      _queues;     

public:
    FramePool(Swapchain::const_pointer swapchain, 
            LogicalDevice::const_pointer device, 
            Pipeline::const_pointer pipe, 
            const QueuePool& queues);
    
    ~FramePool();

    const Frame& operator [](int i);

    size_t size();
};


FramePool::FramePool( Swapchain::const_pointer swapchain, 
                     LogicalDevice::const_pointer device, 
                     Pipeline::const_pointer pipe, 
                     const QueuePool& queues): _ld(device), _queues(queues) {
    std::vector<VkImage>                        images;
    // std::vector<VkCommandBuffer>                buffs;

    VkGet<vkGetSwapchainImagesKHR>(images, device->get(), swapchain->get());

    _frames.reserve(images.size());
    // buffs.resize(images.size());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queues[QueueType::Graphics].getIndex();
    poolInfo.flags = 0; // Optional

    if (vkCreateCommandPool(device->get(), &poolInfo, nullptr, &_native) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }

    for(uint32_t i =0; i < images.size(); ++i) {
        _frames.emplace_back(images[i], _native, swapchain, device, pipe);
    }
}

FramePool::~FramePool() {
    vkDestroyCommandPool(_ld->get(), _native, nullptr);
}

const Frame& FramePool::operator [](int i) {
    return _frames[i];
}

size_t FramePool::size() {
    return _frames.size();
}