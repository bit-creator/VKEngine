export module Geometry;

export import Math.Vector2;
export import Math.Vector3;

import <vector>;
import <array>;
import <memory>;

import Vulkan;
import Vk.VertexBuffer;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;
import Vk.CommandBuffer;
import Vk.HostAllocator;
import Vk.Memory;
import Vk.CommandPool;
export import Geometry.Attributes;

export namespace Vk::Geometry {
    struct FastLoad {
        static inline LogicalDevice   logical  = LogicalDevice::invalid();
        static inline PhysicalDevice  physical = PhysicalDevice::invalid();
        static inline TransferCmdPool transfer = TransferCmdPool::invalid();
    };
};

export struct MemoryRegion {
    size_t offset;
    size_t size;
};

export struct Geometry {
    LocalBuffer                                               vbo;
    Attributes                                                vao;
    std::array<MemoryRegion, 2>                               regions;

public:
    Geometry();
};

export using GeomRef = std::shared_ptr<Geometry>;

using namespace Vk::Geometry;

Geometry::Geometry()
    : vbo(FastLoad::logical)
    , vao() {
    Alloc::HostAllocatorRequirement::logical  = FastLoad::logical;
    Alloc::HostAllocatorRequirement::physical = FastLoad::physical;
}