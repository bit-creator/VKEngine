export module Geometry.Quad;

import <vector>;
import <array>;
import Vulkan;
import Vk.VertexBuffer;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;
import Vk.CommandBuffer;
import Vk.CommandPool;
import Vk.HostAllocator;
import Vk.Memory;
import Vk.CommandPool;
export import Math.Vector2f;
export import Geometry;

export struct Quad: public Geometry {
    Quad();
};

using namespace Vk::Geometry;

Quad::Quad(): Geometry() {
    struct vertex {
        mathon::Vector3f        position;
        // mathon::Vector3f        color;
        mathon::Vector2f        uv;
    };

    std::vector<vertex, Alloc::HostAllocator<vertex>> coord = {
        {{-0.5f, -0.5f, 0.0},   {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0},   {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0},   {1.0f, 1.0f}},
        {{-0.5f,  0.5f, 0.0},   {0.0f, 1.0f}}
    };

    const std::vector<uint16_t, Alloc::HostAllocator<uint16_t>> indices = {
        0, 1, 2, 2, 3, 0
    };

    auto alloc = coord.get_allocator();
    auto idx_alloc = indices.get_allocator();

    vbo.allocate(
        FastLoad::logical, FastLoad::physical,
        sizeof(vertex) * coord.size() + sizeof(uint16_t) * indices.size()
    );

    regions[0] = {0,                sizeof(vertex) * coord.size()};
    regions[1] = {regions[0].size,  sizeof(uint16_t) * indices.size()};

    VkBufferCopy cp_info[2] = {
        {0, regions[0].offset, regions[0].size},
        {0, regions[1].offset, regions[1].size}
    };

    CommandBuffer buff {
        FastLoad::transfer,
        FastLoad::logical
    };

    buff.recordAndSubmit(FastLoad::logical.queues.transfer,
        [&alloc, &idx_alloc, this, &cp_info](CommandBuffer cmd) {
            vkCmdCopyBuffer(cmd, alloc.host, this->vbo, 1, &cp_info[0]);
            vkCmdCopyBuffer(cmd, idx_alloc.host, this->vbo, 1, &cp_info[1]);
        }
    );

    vao.add(Attribute::Position, offsetof(vertex, position), VK_FORMAT_R32G32B32_SFLOAT);
    vao.add(Attribute::Texture, offsetof(vertex, uv), VK_FORMAT_R32G32_SFLOAT);

    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    vao.setBindingDescription(bindingDescription);
    
}