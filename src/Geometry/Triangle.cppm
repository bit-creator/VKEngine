export module Geometry.Triangle;

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

export struct Triangle: public Geometry {
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

    VkBufferCopy cp_info{};
    cp_info.srcOffset = 0; // Optional
    cp_info.dstOffset = 0; // Optional
    cp_info.size = sizeof(Vertex) * coord.size();

    buff.recordAndSubmit(ld.queues.transfer,
        [&alloc, this, &cp_info](CommandBuffer cmd) {
            vkCmdCopyBuffer(buff, alloc.host, vbo, 1, &cp_info);
        }
    );
}