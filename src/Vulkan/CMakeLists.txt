include(${CMAKE_SCRIPTS_DIR}/precompile.cmake)

add_subdirectory(Helpers)

precompile ( Vk.LogicalDevice   VkLogicDevice.cppm )
precompile ( Vk.PhysicalDevice  VkPhysDevice.cppm  )
precompile ( Vk.Swapchain       VkSwapchain.cppm   )
precompile ( Vk.Instance        VkInstance.cppm    )
precompile ( Vk.WindowSurface   VkSurface.cppm     )
precompile ( Vk.Queue           VkQueue.cppm       )

set(sources ${sources} PARENT_SCOPE)
