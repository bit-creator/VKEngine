export module Geometry.Triangle;

export import Math.Vector2;
export import Math.Vector3;

import <vector>;
import <array>;
import Vulkan;
import Vk.VertexBuffer;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;
import Vk.CommandBuffer;
import Vk.HostAllocator;
import Vk.Memory;

export class Vertex {
public:
    Vertex( mathon::Vector2f pos,
            mathon::Vector3f col): position(pos), color(col) {}
    mathon::Vector2f        position;
    mathon::Vector3f        color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        return attributeDescriptions;
    }
};

export struct MemoryRegion {
    size_t offset;
    size_t size;
};

export struct Geometry {
    LocalBuffer                                               vbo;
    std::array<VkVertexInputAttributeDescription, 2>          vao;
    std::array<MemoryRegion, 2>                               regions;
    
public:
    Geometry(LogicalDevice ld, PhysicalDevice pd);
};

Geometry::Geometry(LogicalDevice ld, PhysicalDevice pd)
    : vbo(ld)
    , vao(Vertex::getAttributeDescriptions()) {
    Alloc::HostAllocatorRequirement::logical = ld;
    Alloc::HostAllocatorRequirement::physical = pd;
}

export struct Triangle: public Geometry {
    // Vertex     coord[3];
    Triangle(LogicalDevice ld, PhysicalDevice pd, CommandBuffer buff);
};

Triangle::Triangle(LogicalDevice ld, PhysicalDevice pd, CommandBuffer buff):
    Geometry(ld, pd) {
    std::vector<Vertex, Alloc::HostAllocator<Vertex>> coord = {
        {{0.0f, -0.5f}, {1.0f, 1.0f, 0.0f}},
        {{0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    auto alloc = coord.get_allocator();

    vbo.allocate(ld, pd, sizeof(Vertex) * coord.size());

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(buff, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = sizeof(Vertex) * coord.size();
    vkCmdCopyBuffer(buff, alloc.host, vbo, 1, &copyRegion);

    vkEndCommandBuffer(buff);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buff.native();

    vkQueueSubmit(ld.queues.graphic, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(ld.queues.graphic);
    // vbo.loadData(ld, pd, buff, coord);
}

export struct Quad: Geometry {
    Quad(LogicalDevice ld, PhysicalDevice pd, CommandBuffer buff);
};

Quad::Quad(LogicalDevice ld, PhysicalDevice pd, CommandBuffer buff): Geometry(ld, pd) {
    std::vector<Vertex, Alloc::HostAllocator<Vertex>> coord = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t, Alloc::HostAllocator<uint16_t>> indices = {
        0, 1, 2, 2, 3, 0
    };

    auto alloc = coord.get_allocator();
    auto idx_alloc = indices.get_allocator();

    vbo.allocate(ld, pd, sizeof(Vertex) * coord.size() + sizeof(uint16_t) * indices.size());

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(buff, &beginInfo);

    regions[0] = {0, 80};
    regions[1] = {80, 96};

    VkBufferCopy coord_cp_reg;
    VkBufferCopy idx_cp_reg;
    coord_cp_reg.srcOffset = 0;
    coord_cp_reg.dstOffset = regions[0].offset;
    coord_cp_reg.size = regions[0].size;

    idx_cp_reg.srcOffset = 0;
    idx_cp_reg.dstOffset = regions[1].offset;
    idx_cp_reg.size = regions[1].size;

    std::cout << coord_cp_reg.srcOffset << '\t' << coord_cp_reg.size << std::endl;
    std::cout << idx_cp_reg.srcOffset << '\t' << idx_cp_reg.size << std::endl;

    vkCmdCopyBuffer(buff, alloc.host, vbo, 1, &coord_cp_reg);
    vkCmdCopyBuffer(buff, idx_alloc.host, vbo, 1, &idx_cp_reg);

    vkEndCommandBuffer(buff);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buff.native();

    vkQueueSubmit(ld.queues.graphic, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(ld.queues.graphic);
}
