export module App.PipelineFactory;

export import Vk.PipelineCache;

import <vector>;
import <unordered_map>;

export enum class PipelineLib {
    VertexInput      =0,
    PreRasterisation =1,
    FragmentShader   =2,
    FragmentOutput   =3
};

export struct DrawInfo {
    struct VertexInputData {
        size_t              attributeHash;
        VkPrimitiveTopology topology;
        auto operator <=> (const VertexInputData&) const =default;
    };

    union { 
        VertexInputData     vertexInputData;
        struct {
            size_t              attributeHash;
            VkPrimitiveTopology topology;
        };
    };

    bool operator == (const DrawInfo& info) const {
        return vertexInputData == info.vertexInputData;
    }
};

export template <  > struct std::hash<DrawInfo::VertexInputData> {
	size_t operator ()(const DrawInfo::VertexInputData& draw) const noexcept {
		// same as boost::hash_combine()
		auto hasher = [](size_t& seed, size_t val) mutable -> void {
			seed ^= val + 0x9e3779b9 + (seed<<6) + (seed>>2);
		};

        size_t seed = draw.attributeHash;

        hasher(seed, (size_t)draw.topology);

		return seed;
	}
};

export template <  > struct std::hash<DrawInfo> {
	size_t operator ()(const DrawInfo& draw) const noexcept {
		// same as boost::hash_combine()
		auto hasher = [](size_t& seed, size_t val) mutable -> void {
			seed ^= val + 0x9e3779b9 + (seed<<6) + (seed>>2);
		};

        size_t seed = std::hash<DrawInfo::VertexInputData>{}(draw.vertexInputData);

        // hasher(seed, (size_t)draw._type);

		return seed;
	}
};

export struct RequiredData {
    struct VertexInputData {
        std::vector<VkVertexInputAttributeDescription>  attributes;
        VkVertexInputBindingDescription                 binding;
    }; 
    
    union { 
        VertexInputData                                 vertexInputData;
        struct {
            std::vector<VkVertexInputAttributeDescription>  attributes;
            VkVertexInputBindingDescription                 binding;
        };
    };
};

export struct PipelineData {
    DrawInfo                info;
    RequiredData            data;
};

export struct VertexInputData {
    DrawInfo::VertexInputData                info;
    RequiredData::VertexInputData            data;
};

export struct PipelineAbstrsct:
        public vk::NativeWrapper <VkPipeline> {
    PipelineAbstrsct() =default;
    PipelineAbstrsct(LogicalDevice device);
};

export template <PipelineLib library> class PipelineTree;

export template<> class PipelineTree<PipelineLib::VertexInput> {
protected:
    std::unordered_map<DrawInfo::VertexInputData, PipelineAbstrsct>            _tree;
    LogicalDevice                                                              _device;

public:
    PipelineTree(LogicalDevice device): _device(device) {  }

    PipelineAbstrsct operator[](const VertexInputData& data) {
        if(!_tree.contains(data.info)) prebuild(data);
        
        return _tree[data.info];
    }

private:
    void prebuild(const VertexInputData& data) {
        VkGraphicsPipelineLibraryCreateInfoEXT libInfo{};
		libInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
		libInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_VERTEX_INPUT_INTERFACE_BIT_EXT;

        // auto bindingDescription =    data.data.getBindingDescription();
        // auto attributeDescriptions = data.data.getDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(data.data.attributes.size());
        vertexInputInfo.pVertexBindingDescriptions = &data.data.binding;
        vertexInputInfo.pVertexAttributeDescriptions = data.data.attributes.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = data.info.topology;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkGraphicsPipelineCreateInfo pipelineCI{};
        pipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCI.flags = VK_PIPELINE_CREATE_LIBRARY_BIT_KHR | VK_PIPELINE_CREATE_RETAIN_LINK_TIME_OPTIMIZATION_INFO_BIT_EXT;
		pipelineCI.pNext = &libInfo;
		pipelineCI.pInputAssemblyState = &inputAssembly;
		pipelineCI.pVertexInputState = &vertexInputInfo;
    
        _tree[data.info] = PipelineAbstrsct(_device);

    	VkCreate<vkCreateGraphicsPipelines>(_device, nullptr, 1, &pipelineCI, nullptr, &_tree[data.info].native());
    };
};


export template<> class PipelineTree<PipelineLib::PreRasterisation> {
protected:
    // std::unordered_map<PipelineAbstrsct>            _tree;
};


export template<> class PipelineTree<PipelineLib::FragmentShader> {
protected:
    // std::unordered_map<PipelineAbstrsct>            _tree;
};


export template<> class PipelineTree<PipelineLib::FragmentOutput> {
protected:
    // std::unordered_map<PipelineAbstrsct>            _tree;
};

export class PipelineFactory {
private:
    // PipelineCache                                   _cache;
    std::unordered_map<DrawInfo, PipelineAbstrsct>  _tree;

public:
    PipelineFactory();
    ~PipelineFactory();

private:
    PipelineAbstrsct get();
};

PipelineAbstrsct::PipelineAbstrsct(LogicalDevice device):
    Internal([&](value_type p){ vkDestroyPipeline(device, p, nullptr); }){
}


PipelineFactory::PipelineFactory() {

}

PipelineFactory::~PipelineFactory() {

}

PipelineAbstrsct PipelineFactory::get() {
    // PipelineAbstrsct library[4];
    // library[VertexInput]      = _vertexInputTree[VertexInputData];
    // library[PreRasterisation] = _preRasterisationTree[PreRasterisationData];
    // library[FragmentShader]   = _fragmentShaderTree[FragmentShaderData];
    // library[FragmentOutput]   = _fragmentOutputTree[FragmentOutputData];
    
    // VkPipelineLibraryCreateInfoKHR libInfo{};
	// libInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LIBRARY_CREATE_INFO_KHR;
	// libInfo.libraryCount = 4;
	// libInfo.pLibraries = &library;

	// VkGraphicsPipelineCreateInfo pipelineInfo{};
	// pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	// pipelineInfo.pNext = &libInfo;
	// pipelineInfo.flags |= VK_PIPELINE_CREATE_LINK_TIME_OPTIMIZATION_BIT_EXT;
    
    // VkCreate<vkCreateGraphicsPipelines>(device, _cache, 1, &pipelineInfo, nullptr, &_native);
}