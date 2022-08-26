include(${CMAKE_SCRIPTS_DIR}/settings.cmake)

add_subdirectory(helpers)
add_subdirectory(pipeline)
add_subdirectory(frame)
add_subdirectory(core)

add_library ( VkWrappersGeneral STATIC
    VkSurface.cppm
    VkSwapchain.cppm
    VkSemaphore.cppm
    VkShader.cppm
    VkFence.cppm
)

add_library (VkWrappers INTERFACE)

target_link_libraries( VkWrappers INTERFACE
    VkWrapperCore
    VkWrapperFrame
    VkWrapperHelpers
    VkWrapperPipeline
    VkWrappersGeneral
)