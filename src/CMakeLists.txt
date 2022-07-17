add_subdirectory(Vulkan)
add_subdirectory(Geometry)
add_subdirectory(Scene)

include(${CMAKE_SCRIPTS_DIR}/precompile.cmake)

precompile ( App.NativeWrapper   NativeWrapper.cppm )
precompile ( App.ShaderFactory   ShaderFactory.cppm )
precompile ( App.RenderDevice    RenderDevice.cppm  )
precompile ( App.Settings        AppSettings.cppm   )
precompile ( App.Window          Window.cppm        )
precompile ( App.Event           Event.cppm        )

# find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)
link_directories(VULKAN_DIR)
link_directories(${BUILD_DIR}/src)

add_compile_options(
    -std=c++${CMAKE_CXX_STANDARD}
    -stdlib=libc++
    -fmodules
    -g3
    -Wall
    -Wextra
    -fmath-errno
    -march=native
    -ffast-math
    -fcxx-exceptions
    -fprebuilt-module-path=${PRECOMPILED_MODULES_DIR}
    -fprebuilt-module-path=${MATHON_MODULES_DIR}
)

add_executable(VKEngine
  main.cpp
  ${sources}
)

target_link_libraries(VKEngine
    glfw
    vulkan
    c++
    3rdparty
)