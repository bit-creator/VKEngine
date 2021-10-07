/**
 * @file VkMultisampling.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on multisampling
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.Multisampler;

import Vulkan;

export class Multisampler {
public:
    Multisampler();
    ~Multisampler();

    VkPipelineMultisampleStateCreateInfo 
    getState();
};

Multisampler::Multisampler() {
}

Multisampler::~Multisampler() {
}

VkPipelineMultisampleStateCreateInfo 
getState() {
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    return multisampling;
}