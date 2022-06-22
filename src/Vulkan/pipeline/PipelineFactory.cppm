export module App.PipelineFactory;
export import App.DrawData;

export import Vk.PipelineCache;
export import App.ShaderFactory; 
export import Vk.Shader;

import <vector>;
import <unordered_map>;

export enum class PipelineLib {
    VertexInput      =0,
    PreRasterisation =1,
    FragmentShader   =2,
    FragmentOutput   =3
};

export struct PipelineAbstrsct:
        public vk::NativeWrapper <VkPipeline> {
    PipelineAbstrsct() =default;
    PipelineAbstrsct(LogicalDevice device);
};

export template <PipelineLib library> class PipelineTree;

export template<> class PipelineTree<PipelineLib::VertexInput> {
protected:
    std::unordered_map<data::InfoVI, PipelineAbstrsct>                          _tree;
    LogicalDevice                                                               _device;

public:
    PipelineTree(LogicalDevice device): _device(device) {  }

    PipelineAbstrsct operator[](const data::VertexInput& data) {
        if(!_tree.contains(data.info)) prebuild(data);
        
        return _tree[data.info];
    }

private:
    void prebuild(const data::VertexInput& data) {
        VkGraphicsPipelineLibraryCreateInfoEXT libInfo{};
		libInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
		libInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_VERTEX_INPUT_INTERFACE_BIT_EXT;

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
    std::unordered_map<data::InfoPR, PipelineAbstrsct>                              _tree;
    LogicalDevice                                                                   _device;
    ShaderFactory&                                                                  _factory;

public:
    PipelineTree(LogicalDevice device, ShaderFactory& factory): _device(device), _factory(factory) {  }

    PipelineAbstrsct operator[](const data::PreRasterisation& data) {
        if(!_tree.contains(data.info)) prebuild(data);
        
        return _tree[data.info];
    }

private:
    void prebuild(const data::PreRasterisation& data) {
        VkGraphicsPipelineLibraryCreateInfoEXT libInfo{};
		libInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
		libInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_PRE_RASTERIZATION_SHADERS_BIT_EXT;
		
        auto viewInfo  = data.data.viewport.getState();
        auto rasterInfo= data.data.rasterizer.getState();

        const auto& vertShader = _factory[{(uint32_t)data.info.shaderIndex, ShaderType::Vertex}];
        auto shader = vertShader.getStage();
    	 	
        VkGraphicsPipelineCreateInfo pipelineCI{};
		pipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCI.pNext = &libInfo;
		pipelineCI.renderPass = data.data.pass;
		pipelineCI.flags = VK_PIPELINE_CREATE_LIBRARY_BIT_KHR | VK_PIPELINE_CREATE_RETAIN_LINK_TIME_OPTIMIZATION_INFO_BIT_EXT;
		pipelineCI.stageCount = 1;
		pipelineCI.pStages = &shader;
		pipelineCI.layout = data.data.layout;
		pipelineCI.pDynamicState = nullptr;
		pipelineCI.pViewportState = &viewInfo;
		pipelineCI.pRasterizationState = &rasterInfo;
		
        _tree[data.info] = PipelineAbstrsct(_device);

    	VkCreate<vkCreateGraphicsPipelines>(_device, nullptr, 1, &pipelineCI, nullptr, &_tree[data.info].native());

        // VkGraphicsPipelineLibraryCreateInfoEXT libInfo{};
		// libInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
		// libInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_VERTEX_INPUT_INTERFACE_BIT_EXT;

        // VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

        // vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        // vertexInputInfo.vertexBindingDescriptionCount = 1;
        // vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(data.data.attributes.size());
        // vertexInputInfo.pVertexBindingDescriptions = &data.data.binding;
        // vertexInputInfo.pVertexAttributeDescriptions = data.data.attributes.data();

        // VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        // inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        // inputAssembly.topology = data.info.topology;
        // inputAssembly.primitiveRestartEnable = VK_FALSE;

        // VkGraphicsPipelineCreateInfo pipelineCI{};
        // pipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		// pipelineCI.flags = VK_PIPELINE_CREATE_LIBRARY_BIT_KHR | VK_PIPELINE_CREATE_RETAIN_LINK_TIME_OPTIMIZATION_INFO_BIT_EXT;
		// pipelineCI.pNext = &libInfo;
		// pipelineCI.pInputAssemblyState = &inputAssembly;
		// pipelineCI.pVertexInputState = &vertexInputInfo;
    
        // _tree[data.info] = PipelineAbstrsct(_device);

    	// VkCreate<vkCreateGraphicsPipelines>(_device, nullptr, 1, &pipelineCI, nullptr, &_tree[data.info].native());
    };

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
    // std::unordered_map<DrawInfo, PipelineAbstrsct>  _tree;

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
    // if(!_tree.contains(data.info)) prebuild(data);
        
    //     return _tree[data.info];

    // PipelineAbstrsct library[4];
    // library[VertexInput]      = _vertexInputTree[data.VertexInputData];
    // library[PreRasterisation] = _preRasterisationTree[data.PreRasterisationData];
    // library[FragmentShader]   = _fragmentShaderTree[data.FragmentShaderData];
    // library[FragmentOutput]   = _fragmentOutputTree[data.FragmentOutputData];
    
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