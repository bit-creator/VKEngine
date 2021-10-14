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

/**
 * @class Window
 * @brief Provides simplified api for working with GLFWwindow
 * 
 */
export class Window:
    public NativeWrapper < GLFWwindow*, Window > {
public:
    /**
     * @brief Construct a new GLFWWindow object
     * 
     * @param name of Window
     */
    Window(const char* name);

    /**
     * @brief release GLFWwindow descriptor 
     * and destroy the Window object
     * 
     */
    ~Window();
}; // Window


/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
Window::Window(const char* name) {
    if (!glfwInit()) {
        throw std::runtime_error("failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
    _native = glfwCreateWindow(windowWidth, windowHeight, name, nullptr, nullptr);
    
    if(_native == nullptr) {
        throw std::runtime_error("failed to create GLFW window");
    }
}

Window::~Window() {
    glfwDestroyWindow(_native);
    glfwTerminate();
}