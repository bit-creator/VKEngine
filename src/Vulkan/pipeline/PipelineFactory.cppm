export module App.PipelineFactory;
export import App.DrawData;

import Vulkan;
export import Vk.PipelineCache;
export import App.ShaderFactory; 
export import Vk.Shader;

import <vector>;
import <unordered_map>;

enum class PipelineLib {
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

template <PipelineLib library> class PipelineTree;

template<> class PipelineTree<PipelineLib::VertexInput> {
protected:
    std::unordered_map<data::InfoVI, PipelineAbstrsct>                          _tree;
    LogicalDevice                                                               _device;

public:
    PipelineTree(LogicalDevice device): _device(device) {  }

    inline PipelineAbstrsct operator[](const data::VertexInput& data) {
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

template<> class PipelineTree<PipelineLib::PreRasterisation> {
protected:
    std::unordered_map<data::InfoPR, PipelineAbstrsct>                              _tree;
    LogicalDevice                                                                   _device;
    ShaderFactory&                                                                  _factory;

public:
    PipelineTree(LogicalDevice device, ShaderFactory& factory): _device(device), _factory(factory) {  }

    inline PipelineAbstrsct operator[](const data::PreRasterisation& data) {
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

        const auto vertShader = _factory[{(uint32_t)data.info.shaderIndex, (uint32_t)data.info.attributeHash, ShaderType::Vertex}];
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
   };
}; // PipelineTree<PipelineLib::PreRasterisation>

template<> class PipelineTree<PipelineLib::FragmentShader> {
protected:
    std::unordered_map<data::InfoFS, PipelineAbstrsct>                          _tree;
    LogicalDevice                                                               _device;
    ShaderFactory&                                                              _factory;

public:
    PipelineTree(LogicalDevice device, ShaderFactory& factory): _device(device), _factory(factory) {  }

    inline PipelineAbstrsct operator[](const data::FragmentShader& data) {
        if(!_tree.contains(data.info)) prebuild(data);
        
        return _tree[data.info];
    }

private:
    void prebuild(const data::FragmentShader& data) {
        VkGraphicsPipelineLibraryCreateInfoEXT libInfo{};
		libInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
		libInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_FRAGMENT_SHADER_BIT_EXT;

        const auto fragShader = _factory[{(uint32_t)data.info.shaderIndex, 0, ShaderType::Fragment}];
        auto shader = fragShader.getStage();

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f;
        multisampling.pSampleMask = nullptr;
        multisampling.alphaToCoverageEnable = VK_FALSE;
        multisampling.alphaToOneEnable = VK_FALSE;
    	
        VkGraphicsPipelineCreateInfo pipelineCI{};
		pipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCI.pNext = &libInfo;
		pipelineCI.flags = VK_PIPELINE_CREATE_LIBRARY_BIT_KHR | VK_PIPELINE_CREATE_RETAIN_LINK_TIME_OPTIMIZATION_INFO_BIT_EXT;
		pipelineCI.stageCount = 1;
		pipelineCI.pStages = &shader;
		pipelineCI.layout = data.data.layout;
		pipelineCI.renderPass = data.data.pass;
		pipelineCI.pMultisampleState = &multisampling;
		pipelineCI.pDepthStencilState = nullptr;

        _tree[data.info] = PipelineAbstrsct(_device);

    	VkCreate<vkCreateGraphicsPipelines>(_device, nullptr, 1, &pipelineCI, nullptr, &_tree[data.info].native());
   };
}; // PipelineTree<PipelineLib::FragmentShader>

template<> class PipelineTree<PipelineLib::FragmentOutput> {
protected:
    std::unordered_map<data::InfoFO, PipelineAbstrsct>                          _tree;
    LogicalDevice                                                               _device;

public:
    PipelineTree(LogicalDevice device): _device(device) {  }

    inline PipelineAbstrsct operator[](const data::FragmentOutput& data) {
        if(!_tree.contains(data.info)) prebuild(data);
        
        return _tree[data.info];
    }

private:
    void prebuild(const data::FragmentOutput& data) {
        VkGraphicsPipelineLibraryCreateInfoEXT libInfo{};
		libInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
		libInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_FRAGMENT_OUTPUT_INTERFACE_BIT_EXT;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f;
        multisampling.pSampleMask = nullptr;
        multisampling.alphaToCoverageEnable = VK_FALSE;
        multisampling.alphaToOneEnable = VK_FALSE;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; 
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkGraphicsPipelineCreateInfo pipelineCI{};
        pipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCI.flags = VK_PIPELINE_CREATE_LIBRARY_BIT_KHR | VK_PIPELINE_CREATE_RETAIN_LINK_TIME_OPTIMIZATION_INFO_BIT_EXT;
		pipelineCI.pNext = &libInfo;
		pipelineCI.layout = data.data.layout;
		pipelineCI.renderPass = data.data.pass;
		pipelineCI.pMultisampleState = &multisampling;
		pipelineCI.pColorBlendState = &colorBlending;
    
        _tree[data.info] = PipelineAbstrsct(_device);

    	VkCreate<vkCreateGraphicsPipelines>(_device, nullptr, 1, &pipelineCI, nullptr, &_tree[data.info].native());
    };
}; // PipelineTree<PipelineLib::FragmentOutput>

export class PipelineFactory {
private:
    LogicalDevice                                               _device;
    ShaderFactory                                               _shaders;
    PipelineCache                                               _cache;

    std::unordered_map<data::DrawInfo, PipelineAbstrsct>        _pipelineTree;

    PipelineTree<PipelineLib::VertexInput>                      _vertexInputTree;
    PipelineTree<PipelineLib::PreRasterisation>                 _preRasterisationTree;
    PipelineTree<PipelineLib::FragmentShader>                   _fragmentShaderTree;
    PipelineTree<PipelineLib::FragmentOutput>                   _fragmentOutputTree;

public:
    PipelineFactory(LogicalDevice device);
    ~PipelineFactory();

    inline PipelineAbstrsct operator[](const data::Draw& data) {
        if(!_pipelineTree.contains(data.info)) build(data);
        
        return _pipelineTree[data.info];
    }

private:
    void build(const data::Draw& data);
};

PipelineAbstrsct::PipelineAbstrsct(LogicalDevice device):
    Internal([&](value_type p){ vkDestroyPipeline(device, p, nullptr); }){
}


PipelineFactory::PipelineFactory(LogicalDevice device)
    : _device               (device)
    , _shaders              (device)
    , _cache                (device)
    , _vertexInputTree      (device)
    , _preRasterisationTree (device, _shaders)
    , _fragmentShaderTree   (device, _shaders)
    , _fragmentOutputTree   (device) {
}

PipelineFactory::~PipelineFactory() {
}

void PipelineFactory::build(const data::Draw& data) {
    VkPipeline library[4];
    library[(int)PipelineLib::VertexInput]      = _vertexInputTree[data];
    library[(int)PipelineLib::PreRasterisation] = _preRasterisationTree[data];
    library[(int)PipelineLib::FragmentShader]   = _fragmentShaderTree[data];
    library[(int)PipelineLib::FragmentOutput]   = _fragmentOutputTree[data];
    
    VkPipelineLibraryCreateInfoKHR libInfo{};
	libInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LIBRARY_CREATE_INFO_KHR;
	libInfo.libraryCount = 4;
	libInfo.pLibraries = library;

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.pNext = &libInfo;
	pipelineInfo.flags |= VK_PIPELINE_CREATE_LINK_TIME_OPTIMIZATION_BIT_EXT;

    _pipelineTree[data.info] = PipelineAbstrsct(_device);

    VkCreate<vkCreateGraphicsPipelines>(_device, _cache, 1, &pipelineInfo, nullptr, &_pipelineTree[data.info].native());
}