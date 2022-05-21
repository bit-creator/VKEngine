export module Geometry.Triangle;

export import Math.Vector2;
export import Math.Vector3;

import <vector>;
import <array>;
import Vulkan;
import Vk.VertexBuffer;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;


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

export struct Geometry {
    VertexBuffer                                            vbo;
    std::array<VkVertexInputAttributeDescription, 2>        vao;
public:
    Geometry(LogicalDevice ld);
};

Geometry::Geometry(LogicalDevice ld)
    : vbo(ld) 
    , vao(Vertex::getAttributeDescriptions())
{  }


export struct Triangle: public Geometry {
    // Vertex     coord[3];
    Triangle(LogicalDevice ld, PhysicalDevice pd);
};

Triangle::Triangle(LogicalDevice ld, PhysicalDevice pd):
    Geometry(ld) {
    std::vector<Vertex> coord = {
        {{0.0f, -0.5f}, {1.0f, 1.0f, 0.0f}},
        {{0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    vbo.loadData(ld, pd, coord);
}