include(${CMAKE_SCRIPTS_DIR}/settings.cmake)

add_library ( VkWrapperPipeline STATIC
    VkColorAttachment.cppm
    VkDynamicStates.cppm
    VkMultisampling.cppm
    VkVertexBuffer.cppm
    VkColorBlender.cppm
    VkRenderPass.cppm
    VkRasterizer.cppm
    VkAssembly.cppm
    VkPipelineCache.cppm
    PipelineFactory.cppm
    drawData.cppm
    VkViewport.cppm
    VkLayout.cppm
    VkMemory.cppm
    VkDescriptorPool.cppm
)