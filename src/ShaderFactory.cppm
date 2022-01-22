
/**
 * @file ShaderFactory.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides shaders manegment
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module App.ShaderFactory;

import Vk.Shader;
import Vk.LogicalDevice;

export import <unordered_map>;
export import <string_view>;
export import <filesystem>;
export import <iostream>;
export import <fstream>;
export import <vector>;
export import <string>;
export import <map>;

namespace fs = std::filesystem;

using shaderData = std::pair<ShaderType, std::string>;

export class ShaderFactory {
private:
    using ShaderName   = std::string;
    using BinSource    = std::vector < std::byte >;
    using BinaryTree   = std::map < ShaderName, BinSource >;

    /* FOR FEATURE THINGS */
    // class DrawData; class Shader;
    // using source       = std::string;
    // using srcArray     = std::vector<source>;
    // using srcTree      = std::map < name, srcArray >;
    // using HashedTree   = std::unordered_map<DrawData, Shader>

private:
    BinaryTree                  _shaderTree;
    const LogicalDevice&        _device;

public:
    ShaderFactory(fs::path path, const LogicalDevice& device);
    ~ShaderFactory();

    Shader operator[](const shaderData& data) const;

    /**
     * Be carefully this create a copy of shader buffer
     */
    Shader operator[](const shaderData& data);

private:
    void loadBinarySource(fs::path path);
    Shader get(const shaderData& data) const;
};

ShaderFactory::ShaderFactory(fs::path path, const LogicalDevice& device) : _device(device) {
    for(const auto& shader: fs::recursive_directory_iterator(path)) {
        if(!shader.is_directory()) {
            loadBinarySource(shader.path());
        }
    }
}

ShaderFactory::~ShaderFactory() {
}


void ShaderFactory::loadBinarySource(fs::path path) {
    std::ifstream shader(path, std::ios::ate | std::ios::binary);

    if (shader.is_open()) {
        size_t size = (size_t)shader.tellg();
        _shaderTree[path.filename()].resize(size);
        shader.seekg(0);
        shader.read((char*)_shaderTree[path.filename()].data(), size);
        shader.close();
    } else {
        throw std::runtime_error("failed to open file!");
    }
}

Shader ShaderFactory::get(const shaderData& data) const {
    if(const auto& [type, name] = data; _shaderTree.contains(name)) {
        return Shader(type, _device, _shaderTree.at(name));
    } else {
        throw std::runtime_error("Unknown name of shader");
    }
}

Shader ShaderFactory::operator[](const shaderData& data) const {
    return get(data); 
}

Shader ShaderFactory::operator[](const shaderData& data) {
    return get(data);
}
