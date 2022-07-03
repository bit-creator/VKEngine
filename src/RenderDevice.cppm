/**
 * @file RenderDevice.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the main functionality of the programm
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module App.RenderDevice;

import <filesystem>;
import <iostream>;
import <vector>;
import <array>;

import Vulkan;
import GLFW;

import App.Settings;
import App.Window;

import Vk.PhysicalDevice;
import Vk.LogicalDevice;
import Vk.WindowSurface;
import Vk.Swapchain;
import Vk.Instance;
import Vk.Pipeline;
import Vk.Queue;
import Vk.QueuePool;
import Vk.FramePool;
import Vk.Frame;
import Vk.Semaphore;
import Vk.CommandBuffer;
import Vk.CommandPool;

import Geometry.Quad;


// to-do:/ improve commandPool abstraction to allow creating transfer cmdBuffer

/**
 * @class RenderDevice
 * @brief Aggregates base Vulkan objects such as: 
 * instance, physical/logical devices etc ...
 *
 * @warning This class provides Singleton pattern, 
 * use device() static function to get an instance
 * 
 */
export class RenderDevice {
private:
    Window                                                 wnd;      
    Instance                                               instance; 
    WindowSurface                                          surface;  
    PhysicalDevice                                         physical;
    LogicalDevice                                          logical;
    ShaderFactory                                          factory;
    Swapchain                                              swapchain;
    TransferCmdPool                                        transferPool;
    Quad                                                   geom;
    Pipeline                                               pipeline;
    FramePool                                              frames;
    Semaphore                                              sync;

private:
    /**
     * @brief Capture native resource,
     * construct a new Render Device object
     * 
     */
    RenderDevice();

    /**
     * @brief cleanup all captured resources, 
     * destroy the Render Device object
     * 
     */
    ~RenderDevice() =default;

    /**
     * @brief all copy/move operation forbidden
     * 
     */
    RenderDevice(const RenderDevice&) =delete;
    RenderDevice& operator =(const RenderDevice&) =delete;

public:
    /**
     * @brief get instance to Render Device Object 
     * 
     * @return RenderDevice& 
     */
    static RenderDevice& device();

    /**
     * @brief provides game loop
     * 
     */
    void execute();

private:
    void render();
}; // RenderDevice

RenderDevice::RenderDevice() 
    : wnd           (name)
    , instance      ()
    , surface       (instance, wnd)
    , physical      (instance)
    , logical       (physical, surface)
    , factory       (logical)
    , swapchain     (physical, logical, surface, wnd)
    , transferPool  (logical)
    , geom          (logical, physical, {transferPool, logical})
    , pipeline      (swapchain, logical, factory, geom.vao)
    , frames        (swapchain, logical, pipeline)
    , sync          (logical) {
        // factory.registerShader("simple.glsl");
}

RenderDevice& RenderDevice::device() {
    static RenderDevice device;
    return device;
}

void RenderDevice::execute() {
    while(!glfwWindowShouldClose(wnd)) {
        glfwPollEvents();
        render();
    }

    vkDeviceWaitIdle(logical);
}

void RenderDevice::render() {
    uint32_t imageIndex;
    vkAcquireNextImageKHR(logical, swapchain, UINT64_MAX, sync, VK_NULL_HANDLE, &imageIndex);
    auto frame = frames[imageIndex];
    
    frame.draw(geom);
    auto& renderEnd = frame.submit(sync, logical.queues);

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderEnd.native();

    VkSwapchainKHR swapChains[] = {swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResults = nullptr; // Optional

    vkQueuePresentKHR(logical.queues.graphic, &presentInfo);
    vkQueueWaitIdle(logical.queues.graphic);
}
