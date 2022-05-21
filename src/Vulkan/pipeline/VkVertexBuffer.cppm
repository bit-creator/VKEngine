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

    VkPipelineVertexInputStateCreateInfo 
    getState();

    template <typename T>
    void loadData(LogicalDevice ld, PhysicalDevice pd, CommandBuffer buff, T& data) {
        loadRawData(ld, pd, buff, std::data(data), std::size(data) * sizeof(typename T::value_type));
    }

private:
    void loadRawData(LogicalDevice ld, PhysicalDevice pd, CommandBuffer buff, void* data, size_t size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, PhysicalDevice pd);
};

VertexBuffer::VertexBuffer(LogicalDevice ld):
        Internal([&](value_type vb){ vkDestroyBuffer(ld, vb, nullptr); }),
        _mem(ld) {
}

VkPipelineVertexInputStateCreateInfo VertexBuffer::getState() {
    // auto bindingDescription = Vertex::getBindingDescription();
    // auto attributeDescriptions = Vertex::getAttributeDescriptions();
    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    
    // vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    // vertexInputInfo.vertexBindingDescriptionCount = 1;
    // vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    // vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    // vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

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

    VertexBuffer stagingBuff{ld};

    complete_buffer (
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        *this
    );

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
}