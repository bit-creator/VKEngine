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
import Vk.Commandbuffer;
import Vk.Semaphore;
import Vk.QueuePool;
import Vk.Queue;

export class Frame:
    public NativeWrapper<VkImage, Frame> {
private:
    LogicalDevice::const_pointer            _device;
    Swapchain::const_pointer                _swapchain;
    Pipeline::const_pointer                 _pipe;
    ImageView                               _view;
    Framebuffer                             _buffer;
    Commandbuffer                           _command;
    Semaphore                               _renderEnd;

public:
    Frame(VkImage img, VkCommandPool pool, 
        Swapchain::const_pointer swapchain, 
        LogicalDevice::const_pointer  device, 
        Pipeline::const_pointer  pipe);

    ~Frame();

    Frame(Frame&& oth);
    // Frame& operator =(Frame&&) =default;

    const Semaphore& submit(const Semaphore& imageSync, const QueuePool& queues) const;
};


Frame::Frame(Frame&& oth):
    _device(std::move(oth._device))
    , _swapchain(std::move(oth._swapchain))
    , _pipe(std::move(oth._pipe))
    , _view(std::move(oth._view))
    , _buffer(std::move(oth._buffer))
    , _command(std::move(oth._command)) 
    , _renderEnd(std::move(oth._device))
 {

}

Frame::Frame(VkImage img, VkCommandPool pool, 
    Swapchain::const_pointer swapchain, 
    LogicalDevice::const_pointer  device, 
    Pipeline::const_pointer  pipe): 
          _device(device)
        , _swapchain(swapchain)
        , _pipe(pipe)
        , _view(img, swapchain->getFormat().format, device)
        , _buffer(swapchain->getExtent(), pipe->getRenderPass(), _view, device)
        , _command(pool, device) 
        , _renderEnd(device) {
    _native = img; 

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = _pipe->getRenderPass();
    renderPassInfo.framebuffer = _buffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = _swapchain->getExtent();
    VkClearValue clearColor;
    clearColor.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    vkCmdBeginRenderPass(_command, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(_command, VK_PIPELINE_BIND_POINT_GRAPHICS, *_pipe);
        vkCmdDraw(_command, 3, 1, 0, 0);
    vkCmdEndRenderPass(_command);
    if (vkEndCommandBuffer(_command) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

Frame::~Frame() {
}

const Semaphore& Frame::submit(const Semaphore& imageSync, const QueuePool& queues) const{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageSync.get();
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_command.get();

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &_renderEnd.get();

    if (vkQueueSubmit(queues[QueueType::Graphics].get(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
    
    return _renderEnd;
}