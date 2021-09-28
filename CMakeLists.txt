cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_COMPILER clang++-13)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(PRECOMPILED_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/precompiled)

project(VKEngine)

add_compile_options(-std=c++${CMAKE_CXX_STANDARD})
add_compile_options(-stdlib=libstdc++)
add_compile_options(-fprebuilt-module-path=${PRECOMPILED_MODULES_DIR})

include(cmakeScripts/precompile.cmake)


precompile( App.Settings        AppSettings.cppm  )
precompile( App.Window          Window.cppm       )
# precompile( Vk.Getter           VkGetter.cppm     )
precompile( Vk.Queue            Vulkan/VkQueue.cppm        )
precompile( Vk.Instance         Vulkan/VkInstance.cppm   )
precompile( Vk.WindowSurface    Vulkan/VkSurface.cppm   )
precompile( Vk.PhysicalDevice   Vulkan/VkPhysDevice.cppm )
precompile( Vk.LogicalDevice    Vulkan/VkLogicDevice.cppm )
precompile( App.RenderDevice    RenderDevice.cppm )

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)
# find_package(Vulkan COMPONENTS glslangValidator)

add_executable(VKEngine
  main.cpp
  ${sources}
)

target_link_libraries(VKEngine
    glfw 
    Vulkan::Vulkan
    Vulkan::Headers
    # dl 
    # pthread 
    # X11 
    # Xxf86vm 
    # Xrandr 
    # Xi
    # glslangValidator
)