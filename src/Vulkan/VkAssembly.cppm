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

export module Vk.Assembly;

import Vulkan;

export class Assembly {
    public:
    Assembly();
    ~Assembly();

    VkPipelineInputAssemblyStateCreateInfo 
    getState();
};

Assembly::Assembly() {
}

Assembly::~Assembly() {
}

VkPipelineInputAssemblyStateCreateInfo getState() {
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    return inputAssembly;
}
