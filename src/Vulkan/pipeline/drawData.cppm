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
        size_t              shaderIndex;
        size_t              attributeHash;
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

    struct InfoFO {
        int32_t align = 0;
        auto operator <=> (const InfoFO&) const =default;
    };

    struct DataFO {
        Layout                  layout;
        RenderPass              pass;
    };

    struct FragmentOutput {
        InfoFO              info;
        DataFO              data;
    };

    struct InfoFS {
        size_t          shaderIndex;
        auto operator <=> (const InfoFS&) const =default;
    };

    struct DataFS {
        Layout                  layout;
        RenderPass              pass;
    };

    struct FragmentShader {
        InfoFS              info;
        DataFS              data;
    };

    struct DrawInfo {
        size_t              attributeHash;
        VkPrimitiveTopology topology;
        size_t              vertexShader;
        size_t              fragmentShader;
        auto operator <=> (const DrawInfo&) const =default;
        operator const InfoVI() const {return {attributeHash, topology}; }
        operator const InfoPR() const {return {vertexShader, attributeHash}; }
        operator const InfoFS() const {return {fragmentShader}; }
        operator const InfoFO() const {return {}; }
    };

    struct DrawData {
        std::vector<VkVertexInputAttributeDescription>  attributes;
        VkVertexInputBindingDescription                 binding;
        Viewport                                        viewport;
        Rasterizer                                      rasterizer;
        Layout                                          layout;
        RenderPass                                      pass;
        operator const DataVI() const {return {attributes, binding}; }
        operator const DataPR() const {return {viewport, rasterizer, layout, pass}; }
        operator const DataFS() const {return {layout, pass}; }
        operator const DataFO() const {return {layout, pass}; }
    };

    struct Draw {
        DrawInfo            info;
        DrawData            data;
        operator const VertexInput()      const {return {info, data}; }
        operator const PreRasterisation() const {return {info, data}; }
        operator const FragmentShader()   const {return {info, data}; }
        operator const FragmentOutput()   const {return {info, data}; }
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
		auto hasher = [](size_t& seed, size_t val) mutable -> void {
			seed ^= val + 0x9e3779b9 + (seed<<6) + (seed>>2);
		};

        size_t seed = draw.shaderIndex;
        hasher(seed, (size_t)draw.attributeHash);

		return seed;
	}
};

export template <  > struct std::hash<data::InfoFS> {
	size_t operator ()(const data::InfoFS& draw) const noexcept {
		// same as boost::hash_combine()
		// auto hasher = [](size_t& seed, size_t val) mutable -> void {
		// 	seed ^= val + 0x9e3779b9 + (seed<<6) + (seed>>2);
		// };

        size_t seed = draw.shaderIndex;

		return seed;
	}
};

export template <  > struct std::hash<data::InfoFO> {
	size_t operator ()(const data::InfoFO& draw) const noexcept {
		// same as boost::hash_combine()
		// auto hasher = [](size_t& seed, size_t val) mutable -> void {
		// 	seed ^= val + 0x9e3779b9 + (seed<<6) + (seed>>2);
		// };

        size_t seed = draw.align;

		return seed;
	}
};

export template <  > struct std::hash<data::DrawInfo> {
	size_t operator ()(const data::DrawInfo& draw) const noexcept {
		// same as boost::hash_combine()
		auto hasher = [](size_t& seed, size_t val) mutable -> void {
			seed ^= val + 0x9e3779b9 + (seed<<6) + (seed>>2);
		};

        size_t seed = draw.attributeHash;

        hasher(seed, (size_t)draw.topology);
        hasher(seed, (size_t)draw.vertexShader);
        hasher(seed, (size_t)draw.fragmentShader);

		return seed;
	}
};
