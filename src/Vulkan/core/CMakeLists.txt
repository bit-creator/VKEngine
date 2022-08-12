include(${CMAKE_SCRIPTS_DIR}/settings.cmake)

add_library ( VkWrapperCore STATIC
    VkPhysDevice.cppm
    VkLogicDevice.cppm
    VkQueuePool.cppm
    VkInstance.cppm
    VkQueue.cppm
)