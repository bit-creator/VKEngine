include(${CMAKE_SCRIPTS_DIR}/precompile.cmake)

precompile ( Vk.Extensions VkExtensions.cppm )
precompile ( Vk.Checker    VkChecker.cppm    )
precompile ( Vk.Getter     VkGetter.cppm     )

set(sources ${sources} PARENT_SCOPE)