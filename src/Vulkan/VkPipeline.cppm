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

import Vk.VertexBuffer;
import Vk.Assembly;
import Vk.Viewport;
import Vk.Rasterizer;
import Vk.Multisampler;
import Vk.Swapchain;
import Vk.DynamicState;
import Vk.ColorBlender;
import Vk.Layout;
import Vk.LogicalDevice;

export class Pipeline {
    VertexBuffer                _verticies;
    Assembly                     _assembly;
    Viewport                    _viewport;
    Rasterizer                  _rasterizer;
    Multisampler                _ms;
    DynamicStates               _dynamic;
    ColorBlender                _blender;
    Layout                      _layout;

public:
    Pipeline(const Swapchain& swapchain, const LogicalDevice& device);
    ~Pipeline();
};


Pipeline::Pipeline(const Swapchain& swapchain, const LogicalDevice& device) 
    : _verticies()
    , _assembly()
    , _viewport(swapchain)
    , _rasterizer()
    , _ms()
    , _dynamic(Dynamic::Viewport, Dynamic::LineWidth)
    , _blender()
    , _layout(device)
{

}

Pipeline::~Pipeline() {

}