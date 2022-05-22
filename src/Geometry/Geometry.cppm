export module Geometry;

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