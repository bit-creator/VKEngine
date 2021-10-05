cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_COMPILER clang++-13)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(PRECOMPILED_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/precompiled)

project(VKEngine)

add_compile_options(-std=c++${CMAKE_CXX_STANDARD})
add_compile_options(-fmodules)
add_compile_options(-stdlib=libc++)
add_compile_options(-fimplicit-modules)
add_compile_options(-fimplicit-module-maps)
add_compile_options(-fprebuilt-module-path=${PRECOMPILED_MODULES_DIR})

include(cmakeScripts/precompile.cmake)

# App
precompile( App.Window          Window.cppm        )
precompile( App.Settings        AppSettings.cppm   )
precompile( App.RenderDevice    RenderDevice.cppm  )
precompile( App.ShaderFactory   ShaderFactory.cppm )

# Vk
precompile( Vk.Queue            Vulkan/VkQueue.cppm       )
precompile( Vk.Shader           Vulkan/VkShader.cppm      )
precompile( Vk.WindowSurface    Vulkan/VkSurface.cppm     )
precompile( Vk.Instance         Vulkan/VkInstance.cppm    )
precompile( Vk.Swapchain        Vulkan/VkSwapchain.cppm   )
precompile( Vk.PhysicalDevice   Vulkan/VkPhysDevice.cppm  )
precompile( Vk.LogicalDevice    Vulkan/VkLogicDevice.cppm )

# Vk Helpers
precompile( Vk.Getter           Vulkan/Helpers/VkGetter.cppm     )
precompile( Vk.Checker          Vulkan/Helpers/VkChecker.cppm    )
precompile( Vk.Extensions       Vulkan/Helpers/VkExtensions.cppm )

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
    # stdc++
    c++
    # dl 
    # pthread 
    # X11 
    # Xxf86vm 
    # Xrandr 
    # Xi
    # glslangValidator
)