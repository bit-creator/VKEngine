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

export class VertexBuffer:
    public vk::NativeWrapper <VkBuffer> {
public:
    Memory                  _mem;

    VertexBuffer(LogicalDevice ld);

    template <typename T>
    void loadData(LogicalDevice ld, PhysicalDevice pd, CommandBuffer buff, T& data) {
        loadRawData(ld, pd, buff, std::data(data), std::size(data) * sizeof(typename T::value_type));
    }

    void allocate(
        LogicalDevice           ld,
        PhysicalDevice          pd,
        size_t                  size,
        VkBufferUsageFlags      usage,
        VkSharingMode           shareMode,
        VkMemoryPropertyFlags   flags
    );

    void gen_staging_buff(LogicalDevice ld, PhysicalDevice pd, size_t size);
    void gen_local_buff(LogicalDevice ld, PhysicalDevice pd, size_t size);

    void loadRawData(LogicalDevice ld, PhysicalDevice pd, CommandBuffer buff, void* data, size_t size);
private:
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, PhysicalDevice pd);
};

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
    allocInfo.memoryTypeIndex = findMemoryType(typeFilter, properties, pd);
    
    VkCreate<vkAllocateMemory>(ld, &allocInfo, nullptr, &_mem.native());
    
    vkBindBufferMemory(ld, _native, _mem, 0);
}

export struct StagingBuffer: public VertexBuffer {
    StagingBuffer(LogicalDevice ld);
    void allocate(LogicalDevice ld, PhysicalDevice pd, size_t size);
};

StagingBuffer::StagingBuffer(LogicalDevice ld): VertexBuffer(ld) {  }

void StagingBuffer::allocate(LogicalDevice ld, PhysicalDevice pd, size_t size) {
    VertexBuffer::allocate(ld, pd, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

export class LocalBuffer {
    
};

VertexBuffer::VertexBuffer(LogicalDevice ld):
        // Internal(),
        // Internal([&](value_type vb){ vkDestroyBuffer(ld, vb, nullptr); }),
        _mem(ld) {
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

void VertexBuffer::loadRawData(LogicalDevice ld, PhysicalDevice pd, CommandBuffer buff, void* data, size_t size) {
    auto complete_buffer = [&size, ld, pd] (
            VkBufferUsageFlags      usage,
            VkSharingMode           shareMode,
            VkMemoryPropertyFlags   flags,
            VertexBuffer&           dst
    ) mutable {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = shareMode;
        VkCreate<vkCreateBuffer>(ld, &bufferInfo, nullptr, &dst.native());

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(ld, dst, &memRequirements);
        uint32_t typeFilter = memRequirements.memoryTypeBits;
        VkMemoryPropertyFlags properties = flags;
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = dst.findMemoryType(typeFilter, properties, pd);

        VkCreate<vkAllocateMemory>(ld, &allocInfo, nullptr, &dst._mem.native());
        
        vkBindBufferMemory(ld, dst, dst._mem, 0);
    };
 //
    VertexBuffer stagingBuff{ld};
 // 
    complete_buffer ( 
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
        VK_SHARING_MODE_EXCLUSIVE, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        *this 
    ); //
    
    complete_buffer (
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuff
    );

    stagingBuff._mem.load(data, size);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(buff, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(buff, stagingBuff, *this, 1, &copyRegion);

    vkEndCommandBuffer(buff);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buff.native();

    vkQueueSubmit(ld.queues.graphic, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(ld.queues.graphic);
} //
//
//


void VertexBuffer::gen_local_buff(LogicalDevice ld, PhysicalDevice pd, size_t size) {
    auto complete_buffer = [&size, ld, pd] (
            VkBufferUsageFlags      usage,
            VkSharingMode           shareMode,
            VkMemoryPropertyFlags   flags,
            VertexBuffer&           dst
    ) mutable {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = shareMode;
        VkCreate<vkCreateBuffer>(ld, &bufferInfo, nullptr, &dst.native());

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(ld, dst, &memRequirements);
        uint32_t typeFilter = memRequirements.memoryTypeBits;
        VkMemoryPropertyFlags properties = flags;
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = dst.findMemoryType(typeFilter, properties, pd);

        VkCreate<vkAllocateMemory>(ld, &allocInfo, nullptr, &dst._mem.native());
        
        vkBindBufferMemory(ld, dst, dst._mem, 0);
    };
 //
    // VertexBuffer stagingBuff{ld};
 // 
    complete_buffer ( 
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
        VK_SHARING_MODE_EXCLUSIVE, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        *this 
    ); //
    
}

void VertexBuffer::gen_staging_buff(LogicalDevice ld, PhysicalDevice pd, size_t size) {
    // auto complete_buffer = [&size, ld, pd] (
    //         VkBufferUsageFlags      usage,
    //         VkSharingMode           shareMode,
    //         VkMemoryPropertyFlags   flags,
    //         VertexBuffer&           dst
    // ) mutable {
    //     VkBufferCreateInfo bufferInfo{};
    //     bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    //     bufferInfo.size = size;
    //     bufferInfo.usage = usage;
    //     bufferInfo.sharingMode = shareMode;

    //     VkCreate<vkCreateBuffer>(ld, &bufferInfo, nullptr, &dst.native());
    //     VkMemoryRequirements memRequirements;
    //     vkGetBufferMemoryRequirements(ld, dst, &memRequirements);
    //     uint32_t typeFilter = memRequirements.memoryTypeBits;
    //     VkMemoryPropertyFlags properties = flags;
    //     VkMemoryAllocateInfo allocInfo{};
    //     allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    //     allocInfo.allocationSize = memRequirements.size;
    //     allocInfo.memoryTypeIndex = dst.findMemoryType(typeFilter, properties, pd);

    //     VkCreate<vkAllocateMemory>(ld, &allocInfo, nullptr, &dst._mem.native());
        
    //     vkBindBufferMemory(ld, dst, dst._mem, 0);
    // };

        std::cout << size << std::endl;
//
//
        std::cout << ld.native() << std::endl;
        //
//    
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        VkCreate<vkCreateBuffer>(ld, &bufferInfo, nullptr, &_native);
//
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(ld, _native, &memRequirements);
        uint32_t typeFilter = memRequirements.memoryTypeBits;
        VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(typeFilter, properties, pd);
//
        VkCreate<vkAllocateMemory>(ld, &allocInfo, nullptr, &_mem.native());
  //      
        vkBindBufferMemory(ld, _native, _mem, 0);
//
    // complete_buffer (
    //     VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    //     VK_SHARING_MODE_EXCLUSIVE,
    //     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    //     *this
    // );
}
 //
