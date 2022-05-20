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

import <array>;
import Vulkan;
import Geometry.Triangle;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;
import Vk.Checker;
import Vk.Memory;

export class VertexBuffer:
    public vk::NativeWrapper <VkBuffer> {
public:
    Memory                  _mem;
    Triangle                _geom;

    VertexBuffer(LogicalDevice ld, PhysicalDevice pd);

    VkPipelineVertexInputStateCreateInfo 
    getState();

private:
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, PhysicalDevice pd);
};

VertexBuffer::VertexBuffer(LogicalDevice ld, PhysicalDevice pd):
        Internal([&](value_type vb){ vkDestroyBuffer(ld, vb, nullptr); }),
        _mem(ld) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(Vertex) * 3;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VkCreate<vkCreateBuffer>(ld, &bufferInfo, nullptr, &_native);

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(ld, _native, &memRequirements);
    uint32_t typeFilter = memRequirements.memoryTypeBits;
    VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(typeFilter, properties, pd);

    VkCreate<vkAllocateMemory>(ld, &allocInfo, nullptr, &_mem.native());

    vkBindBufferMemory(ld, _native, _mem, 0);

    void* data;
    vkMapMemory(ld, _mem, 0, bufferInfo.size, 0, &data);
        memcpy(data, &_geom.coord, (size_t) bufferInfo.size);
    vkUnmapMemory(ld, _mem);
}

VkPipelineVertexInputStateCreateInfo VertexBuffer::getState() {
    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();
    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    return vertexInputInfo;
}

uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, PhysicalDevice pd) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(pd, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    return 0;
}