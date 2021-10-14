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

import <vector>;
import <filesystem>;

import Vk.Checker;
import Vk.Shader;
import Vk.Layout;
import Vk.Assembly;
import Vk.Viewport;
import Vk.FramePool;
import Vk.Swapchain;
import Vk.RenderPass;
import Vk.Rasterizer;
import Vk.Multisampler;
import Vk.VertexBuffer;
import Vk.DynamicState;
import Vk.ColorBlender;
import Vk.LogicalDevice;

import App.Settings;
import App.ShaderFactory;

/**
 * @brief simple aggregator for pipeline primitives 
 * 
 */
export class Pipeline:
    public NativeWrapper <VkPipeline, Pipeline> {
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
    FramePool                           _pool;
    ShaderFactory                       _factory;
    LogicalDevice::const_pointer        _device;

public:
    Pipeline(Swapchain::const_pointer swapchain, LogicalDevice::const_pointer device);
    ~Pipeline();
}; // Pipeline


/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
Pipeline::Pipeline(Swapchain::const_pointer swapchain, LogicalDevice::const_pointer device) 
    : _device(device)
    , _verticies()
    , _assembly()
    , _viewport(*swapchain)
    , _rasterizer()
    , _ms()
    , _dynamic(Dynamic::Viewport, Dynamic::LineWidth)
    , _blender()
    , _layout(*device)
    , _pass(*device, *swapchain)
    , _pool(*swapchain, *device, _pass)
    , _factory(std::filesystem::current_path().concat(shaderDirectory), *device)
{
    const auto& vertShader = _factory[{ShaderType::Vertex, "vert.spv"}];
    const auto& fragShader = _factory[{ShaderType::Fragment, "frag.spv"}];

    std::vector stages{vertShader.getStage(), fragShader.getStage()};

    auto vertInfo = _verticies.getState();
    auto assemInfo = _assembly.getState();
    auto viewInfo = _viewport.getState();
    auto rasterInfo= _rasterizer.getState();
    auto msInfo = _ms.getState();
    auto dynInfo = _dynamic.getState();
    auto blendInfo = _blender.getState();
    
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = stages.data();
    pipelineInfo.pVertexInputState    = &vertInfo;
    pipelineInfo.pInputAssemblyState  = &assemInfo;
    pipelineInfo.pViewportState       = &viewInfo;
    pipelineInfo.pRasterizationState  = &rasterInfo;
    pipelineInfo.pMultisampleState    = &msInfo;
    pipelineInfo.pDepthStencilState   = nullptr;
    pipelineInfo.pColorBlendState     = &blendInfo;
    pipelineInfo.pDynamicState        = nullptr;
    pipelineInfo.layout               = _layout;
    pipelineInfo.renderPass           = _pass;

    int magic =1;
    
    VkCreate<vkCreateGraphicsPipelines>(device->get(), nullptr, 1, &pipelineInfo, nullptr, &_native);
}

Pipeline::~Pipeline() {
    vkDestroyPipeline(_device->get(), _native, nullptr);
}