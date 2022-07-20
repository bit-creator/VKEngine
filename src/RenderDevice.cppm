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
import Image;

import App.Settings;
import App.Window;
import Math.Matrix4f;
import App.Event;

import Vk.PhysicalDevice;
import Vk.LogicalDevice;
import Vk.WindowSurface;
import Vk.Swapchain;
import Vk.Instance;
import Vk.VertexBuffer;
import Vk.Queue;
import Vk.QueuePool;
import Vk.FramePool;
import Vk.Frame;
import Vk.Semaphore;
import Vk.CommandBuffer;
import Vk.Framebuffer;
import Vk.CommandPool;
import App.DrawData;
import Vk.Image;
import App.PipelineFactory;
import Scene;
import App.Texture2D;
import Vk.DescriptorPool;
import Material;

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

namespace fs = std::filesystem;

export class RenderDevice {
private:
    Window                                                 wnd;      
    Instance                                               instance; 
    WindowSurface                                          surface;  
    PhysicalDevice                                         physical;
    LogicalDevice                                          logical;
    PipelineFactory                                        pipelines;
    Swapchain                                              swapchain;
    TransferCmdPool                                        transferPool;
    RenderPass                                             pass;
    FramePool                                              frames;
    Semaphore                                              sync;
    DescriptorPool                                         pool;
    TextureRef                                             img;

    ScenePtr                                               scene;
    std::vector<EventPtr>                                  events;

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
    void execute(ScenePtr s);

    void addEvent(EventPtr ptr) { events.push_back(ptr); }
    void removeEvent(EventPtr ptr) { /*for feature work*/ }

private:
    void render(const Frame& frame);
}; // RenderDevice

RenderDevice::RenderDevice()
    : wnd           (name)
    , instance      ()
    , surface       (instance, wnd)
    , physical      (instance)
    , logical       (physical, surface)
    , pipelines     (logical)
    , swapchain     (physical, logical, surface, wnd)
    , transferPool  (logical)
    , pass          (logical, swapchain.getFormat().format)
    , frames        (swapchain, logical, pass)
    , sync          (logical)
    , pool          (logical)
    , img           (new Texture2D(fs::path("assets/textures/brick+wall-512x512.jpg"), logical, physical, {transferPool, logical}, pool.allocate())) {
        Vk::Geometry::FastLoad::logical  = logical;
        Vk::Geometry::FastLoad::physical = physical;
        Vk::Geometry::FastLoad::transfer = transferPool;
        std::cout << "Render device constructed" << std::endl;
}

RenderDevice& RenderDevice::device() {
    static RenderDevice device;
    return device;
}

void RenderDevice::execute(ScenePtr s) {
    scene = s;
    // scene->_object->_material->_albedo = img;
    while(!glfwWindowShouldClose(wnd)) {
        glfwPollEvents();
        uint32_t imageIndex;
        vkAcquireNextImageKHR(logical, swapchain, UINT64_MAX, sync, VK_NULL_HANDLE, &imageIndex);
        auto frame = frames[imageIndex];


        render(frame);

        auto& available = frame.submit(sync, logical.queues);

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &available.native();
    
        VkSwapchainKHR swapChains[] = {swapchain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
    
        presentInfo.pResults = nullptr; // Optional
    
        vkQueuePresentKHR(logical.queues.graphic, &presentInfo);
        vkQueueWaitIdle(logical.queues.graphic);
    }

    vkDeviceWaitIdle(logical);
}

void RenderDevice::render(const Frame& frame) {
    for (auto event: events) event->onRender();
    auto geom = scene->_object->_geometry;
    data::DrawInfo info;
    info.attributeHash = geom->vao.getAttribHash();
    info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    info.vertexShader =0;
    info.fragmentShader =0;

    Viewport viewport(swapchain.getExtent());
    Rasterizer rasterizer;
    Layout layout(logical, pool.layout());

    data::DrawData data {
        geom->vao.getDescriptions()
        , geom->vao.getBindingDescription()
        , viewport
        , rasterizer
        , layout
        , pass
    };

    auto pipeline = pipelines[{info, data}];

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = pass;
    renderPassInfo.framebuffer = frame._buffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapchain.getExtent();
    VkClearValue clearColor;
    clearColor.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    
    frame.bind();
        VkBuffer vertexBuffers[] = {geom->vbo.native()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(frame._command, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(frame._command, vertexBuffers[0], geom->regions[1].offset, VK_INDEX_TYPE_UINT16);
        
    vkCmdBeginRenderPass(frame._command, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(frame._command, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

        mathon::Matrix4f model = scene->_object->transformation(); 

        Transform transf{model};

        vkCmdBindDescriptorSets(frame._command, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &img->_set, 0, nullptr);
        vkCmdPushConstants(frame._command, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Transform), &transf);

        vkCmdDrawIndexed(frame._command, static_cast<uint32_t>(6), 1, 0, 0, 0);

    vkCmdEndRenderPass(frame._command);
    
    frame.unbind();
}
