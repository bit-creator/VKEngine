include(${CMAKE_SCRIPTS_DIR}/precompile.cmake)

add_subdirectory(helpers)
add_subdirectory(pipeline)
add_subdirectory(frame)
add_subdirectory(core)

precompile ( Vk.WindowSurface   VkSurface.cppm         )
precompile ( Vk.Swapchain       VkSwapchain.cppm       )
precompile ( Vk.Semaphore       VkSemaphore.cppm       )
precompile ( Vk.Shader          VkShader.cppm          )

set(sources ${sources} PARENT_SCOPE)
