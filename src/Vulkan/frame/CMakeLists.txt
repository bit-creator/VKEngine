include(${CMAKE_SCRIPTS_DIR}/settings.cmake)

add_library ( VkWrapperFrame STATIC
    VkCommandBuff.cppm
    cmdPool.cppm
    VkFramebuffer.cppm
    DepthBuffer.cppm
    VkFramePool.cppm
    VkImageView.cppm
    VkFrame.cppm
    VkImage.cppm
    VkSampler.cppm
    Texture.cppm
)