export module Geometry.Quad;

import <vector>;
import <array>;
import Vulkan;
import Vk.VertexBuffer;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;
import Vk.CommandBuffer;
import Vk.HostAllocator;
import Vk.Memory;
export import Geometry;

export struct Quad: public Geometry {
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

    regions[0] = {0,                sizeof(Vertex) * coord.size()};
    regions[1] = {regions[0].size,  sizeof(uint16_t) * indices.size()};

    VkBufferCopy cp_info[2] = {
        {0, regions[0].offset, regions[0].size},
        {0, regions[1].offset, regions[1].size}
    };

    buff.recordAndSubmit(ld.queues.transfer,
        [&alloc, &idx_alloc, this, &cp_info](CommandBuffer cmd) {
            vkCmdCopyBuffer(cmd, alloc.host, this->vbo, 1, &cp_info[0]);
            vkCmdCopyBuffer(cmd, idx_alloc.host, this->vbo, 1, &cp_info[1]);
        }
    );
}