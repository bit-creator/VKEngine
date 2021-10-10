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

import Vulkan;

import <vector>;
import <filesystem>;

import Vk.Shader;
import Vk.Layout;
import Vk.Assembly;
import Vk.Viewport;
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

export class Pipeline {
    VertexBuffer                _verticies;
    Assembly                     _assembly;
    Viewport                    _viewport;
    Rasterizer                  _rasterizer;
    Multisampler                _ms;
    DynamicStates               _dynamic;
    ColorBlender                _blender;
    Layout                      _layout;
    RenderPass                  _pass;
    VkPipeline                  _pipeline;
    const LogicalDevice&        _device;
    ShaderFactory               _factory;

public:
    Pipeline(const Swapchain& swapchain, const LogicalDevice& device);
    ~Pipeline();
};


Pipeline::Pipeline(const Swapchain& swapchain, const LogicalDevice& device) 
    : _device(device)
    , _verticies()
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

    auto vertInfo = _verticies.getState();
    auto assemInfo = _assembly.getState();
    auto viewInfo = _viewport.getState();
    auto rasterInfo= _rasterizer.getState();
    auto msInfo = _ms.getState();
    auto dynInfo = _dynamic.getState();
    auto blendInfo = _blender.getState();


    //     VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    // colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    // colorBlendAttachment.blendEnable = VK_FALSE;
    // colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; 
    // colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    // colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    // colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    // colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    // colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    // VkPipelineColorBlendStateCreateInfo colorBlending{};
    // colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    // colorBlending.logicOpEnable = VK_FALSE;
    // colorBlending.logicOp = VK_LOGIC_OP_COPY;
    // colorBlending.attachmentCount = 1;
    // colorBlending.pAttachments = &colorBlendAttachment;
    // colorBlending.blendConstants[0] = 0.0f;
    // colorBlending.blendConstants[1] = 0.0f;
    // colorBlending.blendConstants[2] = 0.0f;
    // colorBlending.blendConstants[3] = 0.0f;
    
    
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
    

    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
}

Pipeline::~Pipeline() {
    vkDestroyPipeline(_device, _pipeline, nullptr);
}