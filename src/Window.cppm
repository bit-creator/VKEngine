/**
 * @file Window.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides a wrapper class for the native GLFW-window
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module App.Window;

export import App.NativeWrapper;

import <stdexcept>;

import GLFW;

import App.Settings;

export struct Window: public 
    vk::NativeWrapper < GLFWwindow* > {
    Window(const char* name);
}; // Window

Window::Window(const char* name)
    : Internal([](GLFWwindow* wnd) {
        glfwDestroyWindow(wnd);
        glfwTerminate();
    }) {
    glfwInit() ?: throw std::runtime_error("failed to initialize GLFW");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
    native(glfwCreateWindow(windowWidth, windowHeight, name, nullptr, nullptr));

    native() ?: throw std::runtime_error("failed to create GLFW window");
}