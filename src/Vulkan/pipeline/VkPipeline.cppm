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
    // PipelineTree<PipelineLib::VertexInput>        _tree;
    // PipelineTree<PipelineLib::PreRasterisation>        _pr;
    // PipelineTree<PipelineLib::FragmentShader>        _fs;
    // PipelineTree<PipelineLib::FragmentOutput>        _fo;

    PipelineFactory             _fac;
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
    , _viewport(swapchain.getExtent())
    , _rasterizer()
    , _ms()
    , _dynamic(Dynamic::Viewport, Dynamic::LineWidth)
    , _blender()
    , _layout(device)
    , _pass(device, swapchain)
    // , _tree(device)
    // , _pr(device, _factory)
    // , _fs(device, _factory)
    // , _fo(device)
    , _fac(device)
{
	// struct PipelineLibrary {
	// 	VkPipeline vertexInputInterface;
	// 	VkPipeline preRasterizationShaders;
    //     VkPipeline fragmentShader;
	// 	VkPipeline fragmentOutputInterface;
	// } pipelineLibrary;

    data::DrawInfo info;
    info.attributeHash = attr.getAttribHash();
    info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    info.vertexShader =0;
    info.fragmentShader =0;

    data::DrawData data {
        attr.getDescriptions()
        , attr.getBindingDescription()
        , _viewport
        , _rasterizer
        , _layout
        , _pass
    };


    // pipelineLibrary.vertexInputInterface = _tree[{info, data}];
    // pipelineLibrary.preRasterizationShaders = _pr[{info, data}];
    // pipelineLibrary.fragmentShader = _fs[{info, data}];
    // pipelineLibrary.fragmentOutputInterface = _fo[{info, data}];

    _native = _fac[{info, data}].native();

    // VkPipelineLibraryCreateInfoKHR pipelineLibraryCI{};
	// pipelineLibraryCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LIBRARY_CREATE_INFO_KHR;
	// pipelineLibraryCI.libraryCount = 4;
	// pipelineLibraryCI.pLibraries = &pipelineLibrary.vertexInputInterface;

	// VkGraphicsPipelineCreateInfo executablePipelineCI{};
	// executablePipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	// executablePipelineCI.pNext = &pipelineLibraryCI;
	// executablePipelineCI.flags |= VK_PIPELINE_CREATE_LINK_TIME_OPTIMIZATION_BIT_EXT;
    
    // VkCreate<vkCreateGraphicsPipelines>(device, nullptr, 1, &executablePipelineCI, nullptr, &_native);
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