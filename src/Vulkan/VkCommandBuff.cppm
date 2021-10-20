/**
 * @file VkCommandBuff.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on command buffers
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

 
export module Vk.Commandbuffer;

export import App.NativeWrapper;

import Vulkan;

import Vk.LogicalDevice;
import Vk.ImageView;
import Vk.RenderPass;

export class Commandbuffer:
    public NativeWrapper<VkCommandBuffer, Commandbuffer> {
public:
    Commandbuffer(VkCommandPool pool, LogicalDevice::const_pointer device);
    ~Commandbuffer();

    Commandbuffer(Commandbuffer&&) =default;
    Commandbuffer& operator =(Commandbuffer&&) =default;
};


Commandbuffer::Commandbuffer(VkCommandPool pool, LogicalDevice::const_pointer device) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(*device, &allocInfo, &_native) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(_native, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
}

Commandbuffer::~Commandbuffer() {
}
