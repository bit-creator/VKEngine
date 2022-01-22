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

export struct WindowSurface: public 
    vk::NativeWrapper < VkSurfaceKHR > {
    WindowSurface(Instance instance, Window window);
}; // WindowSurface

WindowSurface::WindowSurface(Instance instance, Window window):
    Internal([&](value_type w){ vkDestroySurfaceKHR(instance, w, nullptr); }) {
    VkCreate<glfwCreateWindowSurface>(instance, window, nullptr, &_native);
}