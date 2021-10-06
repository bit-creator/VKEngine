/**
 * @file VkAssembly.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on assembly
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.Viewport;

import Vk;
import Vk.Swapchain;

export class Viewport {
private:
    VkViewport          _viewport;
    VkRect2D            _scissor;

public:
    Viewport(const Swapchain& swapchain);
    ~Viewport();

    VkPipelineViewportStateCreateInfo 
    getState();
};

Viewport::Viewport(const Swapchain& swapchain) {
    _viewport.x = 0.0f;
    _viewport.y = 0.0f;
    _viewport.width = (float) swapchain.getExtent().width;
    _viewport.height = (float) swapchain.getExtent().height;
    _viewport.minDepth = 0.0f;
    _viewport.maxDepth = 1.0f;

    _scissor.offset = {0, 0};
    _scissor.extent = swapchain.getExtent();
}

Viewport::~Viewport() {

}

VkPipelineViewportStateCreateInfo 
Viewport::getState() {
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &_viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &_scissor;

    return viewportState;
}