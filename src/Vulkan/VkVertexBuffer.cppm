/**
 * @file VkVertexBuffer.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on vertex buffer
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.VertexBuffer;

import Vulkan;

export class VertexBuffer {
public:
    VertexBuffer();
    ~VertexBuffer();

    VkPipelineVertexInputStateCreateInfo 
    getState();
};

VertexBuffer::VertexBuffer() {
}

VertexBuffer::~VertexBuffer() {
}

VkPipelineVertexInputStateCreateInfo getState() {
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    return vertexInputInfo;
}