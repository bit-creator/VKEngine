/**
 * @file VkFrame.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on frame build process
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.Frame;

export import App.NativeWrapper;

import <array>;
import <vector>;
import <iostream>;

import Vulkan;
import Vk.Swapchain;
import Vk.LogicalDevice;
import Vk.RenderPass;
import Vk.ImageView;
import Vk.Framebuffer;
import Vk.CommandBuffer;
import Vk.Semaphore;
import Vk.QueuePool;
import Vk.Queue;
import Geometry;
import Math.Matrix4f;

export class Frame:
    public vk::NativeWrapper<VkImage> {
public:
    ImageView                               _view;
    Framebuffer                             _buffer;
    CommandBuffer                           _command;
    Semaphore                               _available;

public:
    Frame(VkImage img, VkCommandPool pool, Swapchain swapchain, LogicalDevice device, const RenderPass& pass);

    void bind() const;
    void unbind() const;

    const Semaphore& submit(const Semaphore& imageSync, QueuePool& queues) const;
    void draw(Geometry geom) const;
};

Frame::Frame(VkImage img, VkCommandPool pool, Swapchain swapchain, LogicalDevice device, const RenderPass& pass):
        Internal()
        , _view(img, swapchain.getFormat().format, device)
        , _buffer(swapchain.getExtent(), pass, _view, device)
        , _command(pool, device) 
        , _available(device) {
    _native = img;
}

const Semaphore& Frame::submit(const Semaphore& imageSync, QueuePool& queues) const {
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageSync.native();
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_command.native();

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &_available.native();

    if (vkQueueSubmit(queues[QueueType::Graphics], 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
    
    return _available;
}

void Frame::bind() const {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(_command.native(), &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

}

void Frame::unbind() const {
    if (vkEndCommandBuffer(_command) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}