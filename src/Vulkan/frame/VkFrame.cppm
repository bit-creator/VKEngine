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

import <vector>;
import <iostream>;

import Vulkan;
import Vk.Swapchain;
import Vk.LogicalDevice;
import Vk.Pipeline;
import Vk.RenderPass;
import Vk.ImageView;
import Vk.Framebuffer;
import Vk.CommandBuffer;
import Vk.Semaphore;
import Vk.QueuePool;
import Vk.Queue;
import Geometry.Triangle;

export class Frame:
    public vk::NativeWrapper<VkImage> {
private:
    LogicalDevice                           _device;
    Swapchain                               _swapchain;
    ImageView                               _view;
    Framebuffer                             _buffer;
    CommandBuffer                           _command;
    Semaphore                               _renderEnd;
    Pipeline&                               _pipe;
    VkExtent2D                              _extent;

public:
    Frame(VkImage img, VkCommandPool pool, Swapchain swapchain, LogicalDevice device, Pipeline& pipe);

    void bind() const;
    void unbind() const;

    const Semaphore& submit(const Semaphore& imageSync, QueuePool& queues) const;
    void draw(Geometry geom) const;
};

Frame::Frame(VkImage img, VkCommandPool pool, Swapchain swapchain, LogicalDevice device, Pipeline& pipe):
        Internal()
        , _device(device)
        , _swapchain(swapchain)
        , _view(img, swapchain.getFormat().format, device)
        , _buffer(swapchain.getExtent(), pipe.getRenderPass(), _view, device)
        , _command(pool, device) 
        , _renderEnd(device)
        , _pipe(pipe)
        , _extent(swapchain.getExtent()) {
    _native = img;
}

const Semaphore& Frame::submit(const Semaphore& imageSync, QueuePool& queues) const{
    // if (vkEndCommandBuffer(_command) != VK_SUCCESS) {
    //     throw std::runtime_error("failed to record command buffer!");
    // }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageSync.native();
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_command.native();

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &_renderEnd.native();

    if (vkQueueSubmit(queues[QueueType::Graphics], 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
    
    return _renderEnd;
}

void Frame::draw(Geometry geom) const {
    // VkCommandBufferBeginInfo beginInfo{};
    // beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    // if (vkBeginCommandBuffer(_command.native(), &beginInfo) != VK_SUCCESS) {
    //     throw std::runtime_error("failed to begin recording command buffer!");
    // }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = _pipe.getRenderPass();
    renderPassInfo.framebuffer = _buffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = _extent;
    VkClearValue clearColor;
    clearColor.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    vkCmdBeginRenderPass(_command, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(_command, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipe);

        VkBuffer vertexBuffers[] = {geom.vbo.native()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(_command, 0, 1, vertexBuffers, offsets);

        vkCmdDraw(_command, 3, 1, 0, 0);
    vkCmdEndRenderPass(_command);
    // if (vkEndCommandBuffer(_command) != VK_SUCCESS) {
    //     throw std::runtime_error("failed to record command buffer!");
    // }
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