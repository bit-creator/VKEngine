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

module;
/**
 * @brief Global Module Fragment - contains preprocesor derectives
 * to support header units, currently compilers dont support 
 * importing header unit 
 *
 */
#include <GLFW/glfw3.h>
#include <csignal>

export module App.Window;

/**
 * @brief import dependencies
 * 
 * !!!------------------ATENTION---------------------!!!
 * !!!ALL DEPENDENCIES MUST BE PRECOMPILED EARLY THEN THIS
 * 
 */
import App.Settings;

/**
 * @class Window
 * @brief Provides simplified api for working with GLFWwindow
 * 
 */
export class Window {
private:
    using NativeWindowPtr = GLFWwindow*;
    
    NativeWindowPtr         _window;                // Pointer to Native Window

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

    /**
     * @brief provides implicit conversion 
     * to native class very usefull in native context
     * 
     * @return NativeWindowPtr 
     */
    operator NativeWindowPtr();
}; // Window


/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
Window::Window(const char* name) {
    if (!glfwInit()) {
    	std::raise(SIGTERM);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
    _window = glfwCreateWindow(windowWidth, windowHeight, name, nullptr, nullptr);
    if(_window == nullptr) {
    	std::raise(SIGTERM);
    }
}

Window::~Window() {
    glfwDestroyWindow(_window);
    glfwTerminate();
}

Window::operator Window::NativeWindowPtr() {
    return _window;
}