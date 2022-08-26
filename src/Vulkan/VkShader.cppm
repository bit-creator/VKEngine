/**
 * @file VkShader.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides abstraction above Vulkan ShaderModules
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.Shader;

import Vulkan;

import <vector>;

import Vk.LogicalDevice;
import App.NativeWrapper;
import Vk.Checker;

export enum class ShaderType {
    Vertex   =VK_SHADER_STAGE_VERTEX_BIT,
    Fragment =VK_SHADER_STAGE_FRAGMENT_BIT,
    Geometry =VK_SHADER_STAGE_GEOMETRY_BIT
    // etc...
};

class ShaderFactory;

export struct Shader:
        public vk::NativeWrapper<VkShaderModule> {
    ShaderType                          _type;

    Shader(ShaderType type, LogicalDevice device, const std::vector<std::byte>& source);
    Shader(const Shader&) =default;
    VkPipelineShaderStageCreateInfo getStage() const;
};

export struct VertexShader: public Shader {
    VertexShader(LogicalDevice device, const std::vector<std::byte>& source): 
    Shader(ShaderType::Vertex, device, source) {  }
};

export struct FragmentShader: public Shader {
    FragmentShader(LogicalDevice device, const std::vector<std::byte>& source): 
    Shader(ShaderType::Fragment, device, source) {  }
};

export struct GeometryShader: public Shader {
    GeometryShader(LogicalDevice device, const std::vector<std::byte>& source): 
    Shader(ShaderType::Geometry, device, source) {  }
};

Shader::Shader(ShaderType type, LogicalDevice device, const std::vector<std::byte>& source)
    : Internal([&](value_type sh){ vkDestroyShaderModule(device, sh, nullptr); }) 
    // : Internal([&](value_type sh){  }) 
    // : Internal()
    , _type(type) {    
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = source.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

    VkCreate<vkCreateShaderModule>(device, &createInfo, nullptr, &_native);
    // if (auto res = vkCreateShaderModule(device, &createInfo, nullptr, &_native); res != VK_SUCCESS) {
    //     if(res == VK_ERROR_OUT_OF_HOST_MEMORY) throw std::runtime_error("failed to create shader module! OUT_OF_HOST_MEMORY");
    //     if(res == VK_ERROR_OUT_OF_DEVICE_MEMORY) throw std::runtime_error("failed to create shader module! OUT_OF_DEVICE_MEMORY");
    //     if(res == VK_ERROR_INVALID_SHADER_NV) throw std::runtime_error("failed to create shader module! INVALID_SHADER_NV");
    // }
}

VkPipelineShaderStageCreateInfo Shader::getStage() const {
    VkPipelineShaderStageCreateInfo stage{};
    stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage.stage = (VkShaderStageFlagBits)_type;
    stage.module = _native;
    stage.pName = "main";
    return stage;
}
