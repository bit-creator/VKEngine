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

import <iostream>;
import <array>;
import <vector>;
import Vulkan;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;
import Vk.Checker;
import Vk.Memory;
import Vk.CommandBuffer;

export {
class VertexBuffer:
    public vk::NativeWrapper <VkBuffer> {
public:
    Memory                  _mem;

    VertexBuffer(LogicalDevice ld);

    void allocate(
        LogicalDevice           ld,
        PhysicalDevice          pd,
        size_t                  size,
        VkBufferUsageFlags      usage,
        VkSharingMode           shareMode,
        VkMemoryPropertyFlags   flags
    );

    void load(void* data, size_t size);
}; // VertexBuffer

struct StagingBuffer: public VertexBuffer {
    StagingBuffer(LogicalDevice ld);
    void allocate(LogicalDevice ld, PhysicalDevice pd, size_t size);
    void allocateAndFill(LogicalDevice ld, PhysicalDevice pd, size_t size, void* data);
}; // StagingBuffer

struct LocalBuffer: public VertexBuffer {
    LocalBuffer(LogicalDevice ld);
    void allocate(LogicalDevice ld, PhysicalDevice pd, size_t size);    
}; // LocalBuffer
}; // export

VertexBuffer::VertexBuffer(LogicalDevice ld):
        // Internal([&](value_type vb){ vkDestroyBuffer(ld, vb, nullptr); }),
        _mem(ld) {
}

void VertexBuffer::allocate(LogicalDevice ld, PhysicalDevice pd, 
        size_t size, VkBufferUsageFlags usage,
        VkSharingMode shareMode, VkMemoryPropertyFlags flags) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = shareMode;

    VkCreate<vkCreateBuffer>(ld, &bufferInfo, nullptr, &_native);
    
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(ld, _native, &memRequirements);

    uint32_t typeFilter = memRequirements.memoryTypeBits;
    VkMemoryPropertyFlags properties = flags;

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = _mem.findMemoryType(typeFilter, properties, pd);
    
    VkCreate<vkAllocateMemory>(ld, &allocInfo, nullptr, &_mem.native());
    
    vkBindBufferMemory(ld, _native, _mem, 0);
}

void VertexBuffer::load(void* data, size_t size) {
    _mem.load(data, size);
}

StagingBuffer::StagingBuffer(LogicalDevice ld): VertexBuffer(ld) {  }

void StagingBuffer::allocate(LogicalDevice ld, PhysicalDevice pd, size_t size) {
    VertexBuffer::allocate(ld, pd, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void StagingBuffer::allocateAndFill(LogicalDevice ld, PhysicalDevice pd, size_t size, void* data) {
    allocate(ld, pd, size);
    _mem.load(data, size);
}

LocalBuffer::LocalBuffer(LogicalDevice ld): VertexBuffer(ld) {  }

void LocalBuffer::allocate(LogicalDevice ld, PhysicalDevice pd, size_t size) {
    VertexBuffer::allocate(ld, pd, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_SHARING_MODE_EXCLUSIVE, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}