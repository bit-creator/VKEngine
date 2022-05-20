include(${CMAKE_SCRIPTS_DIR}/precompile.cmake)

precompile ( Vk.PhysicalDevice  VkPhysDevice.cppm  )
precompile ( Vk.LogicalDevice   VkLogicDevice.cppm )
precompile ( Vk.QueuePool       VkQueuePool.cppm   )
precompile ( Vk.Instance        VkInstance.cppm    )
precompile ( Vk.Queue           VkQueue.cppm       )

set(sources ${sources} PARENT_SCOPE)