include(${CMAKE_SCRIPTS_DIR}/settings.cmake)

add_subdirectory(Vulkan)
add_subdirectory(Geometry)
add_subdirectory(Scene)

find_package(glfw3 3.3 REQUIRED)
link_directories(VULKAN_DIR)
link_directories(${BUILD_DIR}/src)

add_executable(VKEngine
  main.cpp
  NativeWrapper.cppm
  ShaderFactory.cppm
  RenderDevice.cppm
  AppSettings.cppm
  Window.cppm
  Event.cppm
)

target_link_libraries(VKEngine
  glfw
  vulkan
  c++
  3rdparty
  Geometry
  Scene
  VkWrappers
)