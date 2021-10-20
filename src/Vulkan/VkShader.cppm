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

export enum class ShaderType {
    Vertex   =VK_SHADER_STAGE_VERTEX_BIT,
    Fragment =VK_SHADER_STAGE_FRAGMENT_BIT,
    Geometry =VK_SHADER_STAGE_GEOMETRY_BIT
    // etc...
};

class ShaderFactory;

export class Shader {
private:
    VkShaderModule                      _shader;
    const LogicalDevice&                _device;
    ShaderType                          _type;


public:
    Shader(ShaderType type, const LogicalDevice& device, const std::vector<std::byte>& source);
    ~Shader();
    VkPipelineShaderStageCreateInfo getStage() const;

private:
    void setupShader(const std::vector<std::byte>& source);

};

export struct VertexShader: public Shader {
    VertexShader(const LogicalDevice& device, const std::vector<std::byte>& source): 
    Shader(ShaderType::Vertex, device, source) {  }
};

export struct FragmentShader: public Shader {
    FragmentShader(const LogicalDevice& device, const std::vector<std::byte>& source): 
    Shader(ShaderType::Fragment, device, source) {  }
};

export struct GeometryShader: public Shader {
    GeometryShader(const LogicalDevice& device, const std::vector<std::byte>& source): 
    Shader(ShaderType::Geometry, device, source) {  }
};

Shader::Shader(ShaderType type, const LogicalDevice& device, const std::vector<std::byte>& source)
    : _device(device), _type(type) {
    setupShader(source);
}

Shader::~Shader() {
    vkDestroyShaderModule(_device, _shader, nullptr);
}

void Shader::setupShader(const std::vector<std::byte>& source) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = source.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

    if (vkCreateShaderModule(_device, &createInfo, nullptr, &_shader) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
}


VkPipelineShaderStageCreateInfo Shader::getStage() const {
    VkPipelineShaderStageCreateInfo stage{};
    stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage.stage = (VkShaderStageFlagBits)_type;
    stage.module = _shader;
    stage.pName = "main";
    return stage;
}
