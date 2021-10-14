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

export import App.NativeWrapper;

import Vulkan;
import GLFW;

import App.Window;

import Vk.Checker;
import Vk.Instance;

/**
 * @class WindowSurface
 * @brief wrap capturing/releasing of VkSurfaceKHR object
 */
export class WindowSurface:
    public NativeWrapper < VkSurfaceKHR, WindowSurface > {
private:
    Instance::const_pointer         _instRef;

public:
    /**
     * @brief capture VkSurfaceKHR
     * 
     * @param instance stored in the class WindowSurface 
     * becouse destruction proccess required Instance
     * @param window used for creating VkSurfaceKHR
     * 
     */
    WindowSurface(Instance::const_pointer instance, Window::const_pointer window);

    /**
     * @brief release VkSurfaceKHR resource
     */
    ~WindowSurface();

}; // WindowSurface



/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
WindowSurface::WindowSurface(Instance::const_pointer instance, Window::const_pointer window) 
    : _instRef(instance) {
    VkCreate<glfwCreateWindowSurface>(instance->get(), window->get(), nullptr, &_native);
}

WindowSurface::~WindowSurface() {
    vkDestroySurfaceKHR(_instRef->get(), _native, nullptr);
}