/**
 * @file VkSurface.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides abstraction above window surface extension
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.WindowSurface;

import Vulkan;
import GLFW;

import Vk.Instance;
import Vk.PhysicalDevice;
import App.Window;

/**
 * @class WindowSurface
 * @brief wrap capturing/releasing of VkSurfaceKHR object
 */
export class WindowSurface {
private:
    VkSurfaceKHR            _surface;
    const Instance&         _instRef;

public:
    /**
     * @brief capture VkSurfaceKHR
     * 
     * @param instance stored in the class WindowSurface 
     * becouse destruction proccess required Instance
     * @param window used for creating VkSurfaceKHR
     * 
     */
    WindowSurface(const Instance& instance, const Window& window);

    /**
     * @brief release VkSurfaceKHR resource
     */
    ~WindowSurface();

    /**
     * @brief incapsulate present queue
     * @param dev 
     * @return present queue index
     */
    uint32_t getPresentQueue(const PhysicalDevice& dev) const;

    /**
     * @brief provides implicit conversion 
     * to native class very usefull in native context
     * 
     * @return VkSurfaceKHR 
     */
    operator VkSurfaceKHR();
    operator VkSurfaceKHR() const;

};

WindowSurface::WindowSurface(const Instance& instance, const Window& window) 
    : _instRef(instance)
{
    // VkCreate<glfwCreateWindowSurface>(instance, window, nullptr, &_surface);
    if (glfwCreateWindowSurface(instance, window, nullptr, &_surface) != VK_SUCCESS) {
    //     // throw std::runtime_error("failed to create window surface!");
    }
}

WindowSurface::~WindowSurface() {
    vkDestroySurfaceKHR(_instRef, _surface, nullptr);
}

uint32_t WindowSurface::getPresentQueue(const PhysicalDevice& dev) const {
    VkBool32 presentSupport = false;
    uint32_t queueIndex;
    vkGetPhysicalDeviceSurfaceSupportKHR(dev, queueIndex, _surface, &presentSupport);
    // if (queueIndex != physical.getQueueIndex(QueueType::Graphics)) {
    //     std::cout << "something went wrong" << std::endl;
    // }
    if(!presentSupport) {
        // std::cout << "Present doesnt Supported" << std::endl;
    }
    return queueIndex;
}


WindowSurface::operator VkSurfaceKHR() {
    return _surface;
}

WindowSurface::operator VkSurfaceKHR() const {
    return _surface;
}