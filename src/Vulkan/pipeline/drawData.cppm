export module App.DrawData;

import Vulkan;

import <iostream>;
import <vector>;

export import Vk.Viewport;
export import Vk.Rasterizer;
export import Vk.Layout;
export import Vk.RenderPass;

export namespace data {
    struct InfoVI {
        size_t              attributeHash;
        VkPrimitiveTopology topology;
        auto operator <=> (const InfoVI&) const =default;
    };

    struct DataVI {
        std::vector<VkVertexInputAttributeDescription>  attributes;
        VkVertexInputBindingDescription                 binding;
    };

    struct VertexInput {
        InfoVI          info;
        DataVI          data;
    };

    struct InfoPR {
        size_t          shaderIndex;
        auto operator <=> (const InfoPR&) const =default;
    };

    struct DataPR {
        Viewport                viewport;
        Rasterizer              rasterizer;
        Layout                  layout;
        RenderPass              pass;
    };
    
    struct PreRasterisation {
        InfoPR              info;
        DataPR              data;
    };
};

export template <  > struct std::hash<data::InfoVI> {
	size_t operator ()(const data::InfoVI& draw) const noexcept {
		// same as boost::hash_combine()
		auto hasher = [](size_t& seed, size_t val) mutable -> void {
			seed ^= val + 0x9e3779b9 + (seed<<6) + (seed>>2);
		};

        size_t seed = draw.attributeHash;

        hasher(seed, (size_t)draw.topology);

		return seed;
	}
};

export template <  > struct std::hash<data::InfoPR> {
	size_t operator ()(const data::InfoPR& draw) const noexcept {
		// same as boost::hash_combine()
		// auto hasher = [](size_t& seed, size_t val) mutable -> void {
		// 	seed ^= val + 0x9e3779b9 + (seed<<6) + (seed>>2);
		// };

        size_t seed = draw.shaderIndex;

		return seed;
	}
};

