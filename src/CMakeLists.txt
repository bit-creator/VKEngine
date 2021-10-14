add_subdirectory(Vulkan)

include(${CMAKE_SCRIPTS_DIR}/precompile.cmake)

precompile ( App.NativeWrapper   NativeWrapper.cppm )
precompile ( App.ShaderFactory   ShaderFactory.cppm )
precompile ( App.RenderDevice    RenderDevice.cppm  )
precompile ( App.Settings        AppSettings.cppm   )
precompile ( App.Window          Window.cppm        )

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_compile_options(
    -std=c++${CMAKE_CXX_STANDARD}
    -fmodules
    -stdlib=libc++
    -fimplicit-modules
    -fimplicit-module-maps
    -fprebuilt-module-path=${PRECOMPILED_MODULES_DIR}
)

add_executable(VKEngine
  main.cpp
  ${sources}
)

target_link_libraries(VKEngine
    glfw 
    Vulkan::Vulkan
    Vulkan::Headers
    c++
)