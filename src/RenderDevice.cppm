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
    Window::const_pointer                                  wnd;      
    Instance::const_pointer                                instance; 
    WindowSurface::const_pointer                           surface;  
    PhysicalDevice::const_pointer                          physical; 
    QueuePool::pointer                                     queues;
    LogicalDevice::const_pointer                           logical;  
    Swapchain::const_pointer                               swapchain;
    Pipeline::const_pointer                                pipeline;


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
    ~RenderDevice();

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
}; // RenderDevice



/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
RenderDevice::RenderDevice() 
    : wnd       (std::make_shared<Window>(name))
    , instance  (std::make_shared<Instance>())
    , surface   (std::make_shared<WindowSurface>(instance, wnd))
    , physical  (std::make_shared<PhysicalDevice>(instance->get()))
    , queues    (std::make_shared<QueuePool>(physical, surface))
    , logical   (std::make_shared<LogicalDevice>(physical, queues))
    , swapchain (std::make_shared<Swapchain>(physical, logical, surface, wnd))
    , pipeline  (std::make_shared<Pipeline>(swapchain, logical))
{  }

RenderDevice::~RenderDevice() {
}

RenderDevice& RenderDevice::device() {
    static RenderDevice device;
    return device;
}

void RenderDevice::execute() {
    while(!glfwWindowShouldClose(*wnd)) {
        glfwPollEvents();
    }
}
