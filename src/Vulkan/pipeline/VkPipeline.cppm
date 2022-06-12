/**
 * @file VkPipeline.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on VkPypeline
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.Pipeline;

export import App.NativeWrapper;

import Vulkan;
import <array>;
import Geometry;

import <vector>;
import <filesystem>;

import App.PipelineFactory;

export import Vk.Checker;
export import Vk.Shader;
export import Vk.Layout;
export import Vk.Assembly;
export import Vk.Viewport;
export import Vk.Swapchain;
export import Vk.QueuePool;
export import Vk.RenderPass;
export import Vk.Rasterizer;
export import Vk.Multisampler;
export import Vk.VertexBuffer;
export import Vk.DynamicState;
export import Vk.ColorBlender;
import Vk.PhysicalDevice;
import Vk.LogicalDevice;
import Geometry.Attributes;

import App.Settings;
export import App.ShaderFactory;

/**
 * @brief simple aggregator for pipeline primitives 
 * 
 */

export class Pipeline:
    public vk::NativeWrapper <VkPipeline> {
private:
    Assembly                            _assembly;
    Viewport                            _viewport;
    Rasterizer                          _rasterizer;
    Multisampler                        _ms;
    DynamicStates                       _dynamic;
    ColorBlender                        _blender;
    Layout                              _layout;
    RenderPass                          _pass;
    PipelineTree<PipelineLib::VertexInput>        _tree;

public:
    Pipeline(Swapchain swapchain, LogicalDevice device, ShaderFactory& _factory, Attributes& attr);

    // const VertexBuffer& getBuffer() const;
    const RenderPass& getRenderPass() const;
    const Layout& getLayout() const;
}; // Pipeline


/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
Pipeline::Pipeline(Swapchain swapchain, LogicalDevice device, ShaderFactory& _factory, Attributes& attr):
    Internal([&](value_type p){ vkDestroyPipeline(device, p, nullptr); })
    , _assembly()
    , _viewport(swapchain)
    , _rasterizer()
    , _ms()
    , _dynamic(Dynamic::Viewport, Dynamic::LineWidth)
    , _blender()
    , _layout(device)
    , _pass(device, swapchain)
    , _tree(device)
{
    VkPushConstantRange push;
    push.offset =0;
    push.size = sizeof(Transform);
    push.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; 
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &push;

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &_layout.native()) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

	struct PipelineLibrary {
		VkPipeline vertexInputInterface;
		VkPipeline preRasterizationShaders;
        VkPipeline fragmentShader;
		VkPipeline fragmentOutputInterface;
	} pipelineLibrary;

    // {
    //     VkGraphicsPipelineLibraryCreateInfoEXT libraryInfo{};
	// 	libraryInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
	// 	libraryInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_VERTEX_INPUT_INTERFACE_BIT_EXT;

    //     auto bindingDescription =    attr.getBindingDescription();
    //     auto attributeDescriptions = attr.getDescriptions();

    //     VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

    //     vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    //     vertexInputInfo.vertexBindingDescriptionCount = 1;
    //     vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    //     vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    //     vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    //     VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    //     inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    //     inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    //     inputAssembly.primitiveRestartEnable = VK_FALSE;

    //     VkGraphicsPipelineCreateInfo pipelineCI{};
    //     pipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	// 	pipelineCI.flags = VK_PIPELINE_CREATE_LIBRARY_BIT_KHR | VK_PIPELINE_CREATE_RETAIN_LINK_TIME_OPTIMIZATION_INFO_BIT_EXT;
	// 	pipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	// 	pipelineCI.pNext = &libraryInfo;
	// 	pipelineCI.pInputAssemblyState = &inputAssembly;
	// 	pipelineCI.pVertexInputState = &vertexInputInfo;
    
    // 	VkCreate<vkCreateGraphicsPipelines>(device, nullptr, 1, &pipelineCI, nullptr, &pipelineLibrary.vertexInputInterface);
    // }

    DrawInfo::VertexInputData info;
    info.attributeHash = attr.getAttribHash();
    info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    RequiredData::VertexInputData data;
    data.binding    = attr.getBindingDescription();
    data.attributes =    attr.getDescriptions();

    pipelineLibrary.vertexInputInterface = _tree[{info, data}];

    {
    	VkGraphicsPipelineLibraryCreateInfoEXT libraryInfo{};
		libraryInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
		libraryInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_PRE_RASTERIZATION_SHADERS_BIT_EXT;
		
        auto viewInfo = _viewport.getState();
        auto rasterInfo= _rasterizer.getState();

        const auto& vertShader = _factory[{0, ShaderType::Vertex}];
        auto shader = vertShader.getStage();


        auto dynInfo = _dynamic.getState();

        // VkDynamicState vertexDynamicStates[2] = {
		// 	VK_DYNAMIC_STATE_VIEWPORT,
		// 	VK_DYNAMIC_STATE_SCISSOR };
		
        // VkPipelineDynamicStateCreateInfo dynamicInfo{};
        // dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		// dynamicInfo.dynamicStateCount = 2;
		// dynamicInfo.pDynamicStates = vertexDynamicStates;

    	 	
        VkGraphicsPipelineCreateInfo pipelineCI{};
		pipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCI.pNext = &libraryInfo;
		pipelineCI.renderPass = _pass;
		pipelineCI.flags = VK_PIPELINE_CREATE_LIBRARY_BIT_KHR | VK_PIPELINE_CREATE_RETAIN_LINK_TIME_OPTIMIZATION_INFO_BIT_EXT;
		pipelineCI.stageCount = 1;
		pipelineCI.pStages = &shader;
		pipelineCI.layout = _layout;
		pipelineCI.pDynamicState = nullptr;
		pipelineCI.pViewportState = &viewInfo;
		pipelineCI.pRasterizationState = &rasterInfo;
		
        VkCreate<vkCreateGraphicsPipelines>(device, nullptr, 1, &pipelineCI, nullptr, &pipelineLibrary.preRasterizationShaders);
    }

    {		
		VkGraphicsPipelineLibraryCreateInfoEXT libraryInfo{};
		libraryInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
		libraryInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_FRAGMENT_OUTPUT_INTERFACE_BIT_EXT;
		
        auto msInfo = _ms.getState();
        auto blendInfo = _blender.getState();
        VkGraphicsPipelineCreateInfo pipelineLibraryCI{};
		pipelineLibraryCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineLibraryCI.flags = VK_PIPELINE_CREATE_LIBRARY_BIT_KHR | VK_PIPELINE_CREATE_RETAIN_LINK_TIME_OPTIMIZATION_INFO_BIT_EXT;
		pipelineLibraryCI.pNext = &libraryInfo;
		pipelineLibraryCI.layout = _layout;
		pipelineLibraryCI.renderPass = _pass;
		pipelineLibraryCI.pColorBlendState = &blendInfo;
		pipelineLibraryCI.pMultisampleState = &msInfo;
        VkCreate<vkCreateGraphicsPipelines>(device, nullptr, 1, &pipelineLibraryCI, nullptr, &pipelineLibrary.fragmentOutputInterface);
	}
    {
    	VkGraphicsPipelineLibraryCreateInfoEXT libraryInfo{};
		libraryInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
		libraryInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_FRAGMENT_SHADER_BIT_EXT;

        auto msInfo = _ms.getState();		
        const auto& fragShader = _factory[{0, ShaderType::Fragment}];
        auto fshader = fragShader.getStage();

		VkGraphicsPipelineCreateInfo pipelineCI{};
		pipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCI.pNext = &libraryInfo;
		pipelineCI.flags = VK_PIPELINE_CREATE_LIBRARY_BIT_KHR | VK_PIPELINE_CREATE_RETAIN_LINK_TIME_OPTIMIZATION_INFO_BIT_EXT;
		pipelineCI.stageCount = 1;
		pipelineCI.pStages = &fshader;
		pipelineCI.layout = _layout;
		pipelineCI.renderPass = _pass;
		pipelineCI.pDepthStencilState = nullptr;
		pipelineCI.pMultisampleState = &msInfo;
        VkCreate<vkCreateGraphicsPipelines>(device, nullptr, 1, &pipelineCI, nullptr, &pipelineLibrary.fragmentShader);
    }
    
    // const auto& vertShader = _factory[{0, ShaderType::Vertex}];
    // const auto& fragShader = _factory[{0, ShaderType::Fragment}];

    // std::vector stages{vertShader.getStage(), fragShader.getStage()};

    // auto bindingDescription = attr.getBindingDescription();
    // auto attributeDescriptions = attr.getDescriptions();

    // VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    
    // vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    // vertexInputInfo.vertexBindingDescriptionCount = 1;
    // vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    // vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    // vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    // // auto vertInfo = _verticies.getState();
    // auto assemInfo = _assembly.getState();
    // auto viewInfo = _viewport.getState();
    // auto rasterInfo= _rasterizer.getState();
    // auto msInfo = _ms.getState();
    // auto dynInfo = _dynamic.getState();
    // auto blendInfo = _blender.getState();

    
    // VkGraphicsPipelineCreateInfo pipelineInfo{};
    // pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    // pipelineInfo.stageCount = 2;
    // pipelineInfo.pStages = stages.data();
    // pipelineInfo.pVertexInputState    = &vertexInputInfo;
    // pipelineInfo.pInputAssemblyState  = &assemInfo;
    // pipelineInfo.pViewportState       = &viewInfo;
    // pipelineInfo.pRasterizationState  = &rasterInfo;
    // pipelineInfo.pMultisampleState    = &msInfo;
    // pipelineInfo.pDepthStencilState   = nullptr;
    // pipelineInfo.pColorBlendState     = &blendInfo;
    // pipelineInfo.pDynamicState        = nullptr;
    // pipelineInfo.layout               = _layout;
    // pipelineInfo.renderPass           = _pass;

    VkPipelineLibraryCreateInfoKHR pipelineLibraryCI{};
	pipelineLibraryCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LIBRARY_CREATE_INFO_KHR;
	pipelineLibraryCI.libraryCount = 4;
	pipelineLibraryCI.pLibraries = &pipelineLibrary.vertexInputInterface;

	VkGraphicsPipelineCreateInfo executablePipelineCI{};
	executablePipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	executablePipelineCI.pNext = &pipelineLibraryCI;
	executablePipelineCI.flags |= VK_PIPELINE_CREATE_LINK_TIME_OPTIMIZATION_BIT_EXT;
    
    VkCreate<vkCreateGraphicsPipelines>(device, nullptr, 1, &executablePipelineCI, nullptr, &_native);
    // VkCreate<vkCreateGraphicsPipelines>(device, nullptr, 1, &pipelineInfo, nullptr, &_native);
}

const RenderPass& Pipeline::getRenderPass() const {
    return _pass;
}

const Layout& Pipeline::getLayout() const {
    return _layout;
}

// const VertexBuffer& Pipeline::getBuffer() const {
//     return _verticies;
// }