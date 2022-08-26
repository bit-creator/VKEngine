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
import Vk.Fence;
import Geometry;
import Math.Matrix4f;
import Vk.DescriptorPool;

export class Frame:
    public vk::NativeWrapper<VkImage> {
public:
    ImageView                               _view;
    Framebuffer                             _buffer;
    CommandBuffer                           _command;
    VkDescriptorSet                         _set;

    Semaphore                               _available;
    Fence                                   _commandEnd;
    LogicalDevice                           _device;

public:
    Frame(VkImage img, VkDescriptorSet set, VkCommandPool pool, const ImageView& db,
        Swapchain swapchain, LogicalDevice device, const RenderPass& pass);

    void bind() const;
    void unbind() const;

    const Semaphore& submit(const Semaphore& imageSync, QueuePool& queues) const;
    void draw(Geometry geom) const;
};

Frame::Frame(VkImage img, VkDescriptorSet set, VkCommandPool pool, const ImageView& db,
            Swapchain swapchain, LogicalDevice device, const RenderPass& pass):
        Internal()
        , _view(img, swapchain.getFormat().format, device)
        , _buffer(swapchain.getExtent(), pass, std::vector<VkImageView>{_view, db}, device)
        , _command(pool, device)
        , _set(set)
        , _available(device)
        , _commandEnd(device)
        , _device(device) {
    _native = img;
}

const Semaphore& Frame::submit(const Semaphore& imageSync, QueuePool& queues) const {
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageSync.native();
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_command.native();

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &_available.native();

    if (vkQueueSubmit(queues[QueueType::Graphics], 1, &submitInfo, _commandEnd) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    return _available;
}

void Frame::bind() const {
    vkWaitForFences(_device, 1, &_commandEnd.native(), VK_TRUE, UINT64_MAX);
    vkResetFences(_device, 1, &_commandEnd.native());

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