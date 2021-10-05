
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

import <unordered_map>;
import <string_view>;
import <filesystem>;
import <iostream>;
import <fstream>;
import <vector>;
import <string>;
import <map>;

namespace fs = std::filesystem;

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

public:
    ShaderFactory(fs::path path);
    ~ShaderFactory();

    const BinSource& operator[](const std::string& name) const;

    /**
     * Be carefully this create a copy of shader buffer
     */
    BinSource operator[](const std::string& name);

private:
    void loadBinarySource(fs::path path);
};

ShaderFactory::ShaderFactory(fs::path path) {
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

const std::vector < std::byte >& ShaderFactory::operator[](const std::string& name) const {
    if(_shaderTree.contains(name)) return _shaderTree.at(name);
    else throw std::runtime_error("Unknown name of shader");
}

std::vector < std::byte > ShaderFactory::operator[](const std::string& name) {
    if(_shaderTree.contains(name)) return _shaderTree[name];
    else throw std::runtime_error("Unknown name of shader");
}
