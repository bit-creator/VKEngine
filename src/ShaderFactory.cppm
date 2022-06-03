
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
import App.Settings;

namespace fs = std::filesystem;

// using shaderData = std::pair<ShaderType, std::string>;

export struct ShaderData {
    ShaderType      _type;
    std::string     _name;

    std::string getFile() const {
        return _name + std::to_string((int)_type) + ".spv";
    }
};

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

    // Shader operator[](const ShaderData& data) const;

    /**
     * Be carefully this create a copy of shader buffer
     */
    Shader operator[](const ShaderData& data);

private:
    void loadBinarySource(fs::path path);
    void compileShader(const ShaderData& data);
    Shader get(const ShaderData& data);
};

ShaderFactory::ShaderFactory(fs::path path, const LogicalDevice& device) : _device(device) {
    // for(const auto& shader: fs::recursive_directory_iterator(path)) {
    //     if(!shader.is_directory()) {
    //         loadBinarySource(shader.path());
    //     }
    // }
}

ShaderFactory::~ShaderFactory() {
}


void ShaderFactory::loadBinarySource(fs::path path) {
    std::ifstream shader(path, std::ios::ate | std::ios::binary);

    if (shader.is_open()) {
        size_t size = (size_t)shader.tellg();
        std::cout << path.filename() << std::endl;
        _shaderTree[path.filename()].resize(size);
        shader.seekg(0);
        shader.read((char*)_shaderTree[path.filename()].data(), size);
        shader.close();
    } else {
        throw std::runtime_error("failed to open file!");
    }
}

Shader ShaderFactory::get(const ShaderData& data) {
    if(_shaderTree.contains(data.getFile())) {
        return Shader(data._type, _device, _shaderTree.at(data.getFile()));
    } else {
        compileShader(data);
        loadBinarySource(std::string(shaderDirectory) + "/bin/" + data.getFile());
        return Shader(data._type, _device, _shaderTree.at(data.getFile()));
    }
}

void ShaderFactory::compileShader(const ShaderData& data) {
    std::string stage = data._type == ShaderType::Vertex ? "vert" : "frag";
    std::string define = data._type == ShaderType::Vertex ? "VERTEX_SHADER" : "FRAGMENT_SHADER";
    
    std::string syscall =
        "glslangValidator -V " +
        std::string(shaderDirectory) + "shaders/" + data._name + ".glsl" +
        " -S " + stage + " -D" + define + " -o " +
        std::string(shaderDirectory) + "bin/" + data.getFile();

    std::cout << syscall << std::endl;
    system(syscall.c_str());
}

Shader ShaderFactory::operator[](const ShaderData& data) {
    return get(data);
}
