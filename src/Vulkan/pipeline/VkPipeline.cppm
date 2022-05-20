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
import Geometry.Triangle;

import <vector>;
import <filesystem>;

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

import App.Settings;
export import App.ShaderFactory;

/**
 * @brief simple aggregator for pipeline primitives 
 * 
 */
export class Pipeline:
    public vk::NativeWrapper <VkPipeline> {
private:
    VertexBuffer                        _verticies;
    Assembly                            _assembly;
    Viewport                            _viewport;
    Rasterizer                          _rasterizer;
    Multisampler                        _ms;
    DynamicStates                       _dynamic;
    ColorBlender                        _blender;
    Layout                              _layout;
    RenderPass                          _pass;
    ShaderFactory                       _factory;

public:
    Pipeline(Swapchain swapchain, LogicalDevice device, PhysicalDevice pd);

    const VertexBuffer& getBuffer() const;
    const RenderPass& getRenderPass() const;
}; // Pipeline


/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
Pipeline::Pipeline(Swapchain swapchain, LogicalDevice device, PhysicalDevice pd):
    Internal([&](value_type p){ vkDestroyPipeline(device, p, nullptr); })
    , _verticies(device, pd)
    , _assembly()
    , _viewport(swapchain)
    , _rasterizer()
    , _ms()
    , _dynamic(Dynamic::Viewport, Dynamic::LineWidth)
    , _blender()
    , _layout(device)
    , _pass(device, swapchain)
    , _factory(std::filesystem::current_path().concat(shaderDirectory), device)
{
    const auto& vertShader = _factory[{ShaderType::Vertex, "vert.spv"}];
    const auto& fragShader = _factory[{ShaderType::Fragment, "frag.spv"}];

    std::vector stages{vertShader.getStage(), fragShader.getStage()};



    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();
    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();





    // auto vertInfo = _verticies.getState();
    auto assemInfo = _assembly.getState();
    auto viewInfo = _viewport.getState();
    auto rasterInfo= _rasterizer.getState();
    auto msInfo = _ms.getState();
    // auto dynInfo = _dynamic.getState();
    auto blendInfo = _blender.getState();
    
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = stages.data();
    pipelineInfo.pVertexInputState    = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState  = &assemInfo;
    pipelineInfo.pViewportState       = &viewInfo;
    pipelineInfo.pRasterizationState  = &rasterInfo;
    pipelineInfo.pMultisampleState    = &msInfo;
    pipelineInfo.pDepthStencilState   = nullptr;
    pipelineInfo.pColorBlendState     = &blendInfo;
    pipelineInfo.pDynamicState        = nullptr;
    pipelineInfo.layout               = _layout;
    pipelineInfo.renderPass           = _pass;
    
    VkCreate<vkCreateGraphicsPipelines>(device, nullptr, 1, &pipelineInfo, nullptr, &_native);
}

const RenderPass& Pipeline::getRenderPass() const {
    return _pass;
}

const VertexBuffer& Pipeline::getBuffer() const {
    return _verticies;
}