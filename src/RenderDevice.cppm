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

import Vulkan;
import GLFW;

import <array>;
import <iostream>;
import <vector>;
import <filesystem>;

import App.Window;
import App.Settings;

import Vk.Instance;
import Vk.Queue;
import Vk.Swapchain;
import Vk.WindowSurface;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;
import Vk.Swapchain;
import Vk.Pipeline;
import App.ShaderFactory;

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
    Window                                  wnd;      
    Instance                                instance; 
    WindowSurface                           surface;  
    PhysicalDevice                          physical; 
    LogicalDevice                           logical;  
    // std::array<Queue, NUM_OF_QUEUE>         descriptors;         
    Queue                                   descriptor;
    Swapchain                               swapchain;
    Pipeline                                pipeline;
    ShaderFactory                           factory;


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
    : wnd(name)
    , instance()
    , surface(instance, wnd)
    , physical(instance)
    , logical(physical)
    // , descriptors({
    //     GraphicQueue(physical, logical)
    // })
    , descriptor(GraphicQueue(physical, logical))
    , swapchain(physical, logical, surface, wnd)
    , pipeline(swapchain, logical)
    , factory(std::filesystem::current_path().concat(shaderDirectory))
{ 
    // if (glfwCreateWindowSurface(instance, wnd, nullptr, &surface) != VK_SUCCESS) {
    //     // LOGIT
    // }
    // VkBool32 presentSupport = false;
    // uint32_t queueIndex;
    // vkGetPhysicalDeviceSurfaceSupportKHR(physical, queueIndex, surface, &presentSupport);
    // if (queueIndex != physical.getQueueIndex(QueueType::Graphics)) {
    //     std::cout << "something went wrong" << std::endl;
    // }
    // std::vector <VkExtensionProperties> vec;
    // Extensions::getAllSupported(vec);
    // for(auto v: vec) {
    //     std::cout << v.extensionName <<std::endl;
    // }
}

RenderDevice::~RenderDevice() {
}

RenderDevice& RenderDevice::device() {
    static RenderDevice device;
    return device;
}

void RenderDevice::execute() {
    while(!glfwWindowShouldClose(wnd)) {
        glfwPollEvents();
    }
}