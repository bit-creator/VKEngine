include(${CMAKE_SCRIPTS_DIR}/precompile.cmake)

precompile ( Vk.CommandBuffer   VkCommandBuff.cppm     )
precompile ( Vk.CommandPool     cmdPool.cppm           )
precompile ( Vk.Framebuffer     VkFramebuffer.cppm     )
precompile ( Vk.FramePool       VkFramePool.cppm       )
precompile ( Vk.ImageView       VkImageView.cppm       )
precompile ( Vk.Frame           VkFrame.cppm           )
precompile ( Vk.Image           VkImage.cppm           )
precompile ( Vk.Image.Sampler   VkSampler.cppm         )
precompile ( Vk.Texture2D       Texture.cppm           )
precompile ( Vk.DepthBuffer     DepthBuffer.cppm       )

set(sources ${sources} PARENT_SCOPE)