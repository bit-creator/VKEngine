add_subdirectory(Vulkan)
add_subdirectory(Geometry)

include(${CMAKE_SCRIPTS_DIR}/precompile.cmake)

precompile ( App.NativeWrapper   NativeWrapper.cppm )
precompile ( App.ShaderFactory   ShaderFactory.cppm )
precompile ( App.RenderDevice    RenderDevice.cppm  )
precompile ( App.Settings        AppSettings.cppm   )
precompile ( App.Window          Window.cppm        )

# find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)
link_directories(VULKAN_DIR)

Message(${VULKAN_DIR})

add_compile_options(
    -std=c++${CMAKE_CXX_STANDARD}
    -std=c++20
    -stdlib=libc++
    -fmodules
    -O3
    -fPIC
    -Wall
    -Wextra
    -fmath-errno
    -march=native
    -ffast-math
    -fcxx-exceptions
    -fprebuilt-module-path=${PRECOMPILED_MODULES_DIR}
    -fprebuilt-module-path=/home/user/Code/C++/Mathon/precompiled/
)

add_executable(VKEngine
  main.cpp
  ${sources}
)

target_link_libraries(VKEngine
    glfw
    vulkan
    # Vulkan::Headers
    c++
    mathon
)