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

 
export module Vk.CommandBuffer;

export import App.NativeWrapper;

import Vulkan;

import Vk.LogicalDevice;
import Vk.RenderPass;
import Vk.ImageView;
import Vk.Checker;
import Vk.Queue;

export struct CommandBuffer:
        public vk::NativeWrapper<VkCommandBuffer> {
    CommandBuffer(VkCommandPool pool, LogicalDevice device);
    
    void submit(Queue queue) {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &_native;

        vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(queue);
    }

    template < typename Pred >
    void record(Pred predicat) const {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        VkCreate<vkBeginCommandBuffer>(_native, &beginInfo);

        predicat(*this);

        vkEndCommandBuffer(_native);
    }

    template < typename Pred >
    void recordAndSubmit(Queue queue, Pred predicat) {
        record(predicat);
        submit(queue);
    }
};

CommandBuffer::CommandBuffer(VkCommandPool pool, LogicalDevice device) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(device, &allocInfo, &_native) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    // VkCommandBufferBeginInfo beginInfo{};
    // beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    // beginInfo.flags = 0; // Optional
    // beginInfo.pInheritanceInfo = nullptr; // Optional

    // if (vkBeginCommandBuffer(_native, &beginInfo) != VK_SUCCESS) {
    //     throw std::runtime_error("failed to begin recording command buffer!");
    // }
}


