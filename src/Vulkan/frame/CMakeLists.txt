include(${CMAKE_SCRIPTS_DIR}/precompile.cmake)

precompile ( Vk.CommandBuffer   VkCommandBuff.cppm     )
precompile ( Vk.Framebuffer     VkFramebuffer.cppm     )
precompile ( Vk.FramePool       VkFramePool.cppm       )
precompile ( Vk.ImageView       VkImageView.cppm       )
precompile ( Vk.Frame           VkFrame.cppm           )

set(sources ${sources} PARENT_SCOPE)