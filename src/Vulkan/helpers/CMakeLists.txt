include(${CMAKE_SCRIPTS_DIR}/settings.cmake)

add_library ( VkWrapperHelpers STATIC
    VkExtensions.cppm
    VkChecker.cppm
    VkGetter.cppm
)