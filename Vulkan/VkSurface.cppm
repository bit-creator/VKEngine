module;
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
export module Vk.WindowSurface;
import Vk.Instance;

export class WindowSurface {
private:
    VkSurfaceKHR            _surface;

public:
    WindowSurface(Instance& instance);
    ~WindowSurface();


};

WindowSurface::WindowSurface(Instance& instance) {
    // if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
    //     throw std::runtime_error("failed to create window surface!");
    // }
}

WindowSurface::~WindowSurface() {

}