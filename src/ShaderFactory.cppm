
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

export struct ShaderData {
    uint32_t        _shaderNumber;
    ShaderType      _type;
    auto operator <=> (const ShaderData&) const =default;
};

export template <  > struct std::hash<ShaderData> {
	size_t operator ()(const ShaderData& draw) const noexcept {
		// same as boost::hash_combine()
		auto hasher = [](size_t& seed, size_t val) mutable -> void {
			seed ^= val + 0x9e3779b9 + (seed<<6) + (seed>>2);
		};

        size_t seed = draw._shaderNumber;

        hasher(seed, (size_t)draw._type);

		return seed;
	}
};

export class ShaderFactory {
private:
    using SpirV        = std::vector < std::byte >;
    using ShaderMap    = std::unordered_map < ShaderData, SpirV >;

private:
    std::vector<fs::path>       _pathes;
    ShaderMap                   _shaders;
    const LogicalDevice&        _device;

public:
    ShaderFactory(fs::path path, const LogicalDevice& device);
    ~ShaderFactory();

    void registerShader(fs::path path);

    /**
     * Be carefully this create a copy of shader buffer
     */
    Shader operator[](const ShaderData& data);

private:
    uint32_t getShaderPathNumber(fs::path path);
    void loadBinarySource(const ShaderData& data);
    void compileShader(const ShaderData& data);
    Shader get(const ShaderData& data);
    std::string spvFile(const ShaderData& data);
};

ShaderFactory::ShaderFactory(fs::path path, const LogicalDevice& device) : _device(device) {
    _pathes.push_back(preRegistredShader);
}

ShaderFactory::~ShaderFactory() {
}

void ShaderFactory::registerShader(fs::path path) {
    _pathes.push_back(path);
};

uint32_t ShaderFactory::getShaderPathNumber(fs::path path) {
    if(auto it = std::find(_pathes.begin(), _pathes.end(), path); it != _pathes.end())
        return std::distance(_pathes.begin(), it);
    throw std::runtime_error(std::string("Shader " + std::string(path) + " doesnt registered"));
}

void ShaderFactory::loadBinarySource(const ShaderData& data) {
    std::ifstream shader(spvFile(data), std::ios::ate | std::ios::binary);

    if (shader.is_open()) {
        size_t size = (size_t)shader.tellg();
        _shaders[data] = SpirV{size};
        shader.seekg(0);
        shader.read((char*)_shaders[data].data(), size);
        shader.close();
    } else {
        throw std::runtime_error("failed to open file!");
    }
}

Shader ShaderFactory::get(const ShaderData& data) {
    if(_shaders.contains(data)) {
        return Shader(data._type, _device, _shaders[data]);
    } else {
        compileShader(data);
        loadBinarySource(data);
        return Shader(data._type, _device, _shaders[data]);
    }
}

std::string ShaderFactory::spvFile(const ShaderData& data) {
    return std::string(binaryDirectory) +
        std::to_string(std::hash<ShaderData>{}(data)) + ".spv";
}

void ShaderFactory::compileShader(const ShaderData& data) {
    std::string stage = data._type == ShaderType::Vertex ? "vert" : "frag";
    std::string define = data._type == ShaderType::Vertex ? "VERTEX_SHADER" : "FRAGMENT_SHADER";
    
    std::string syscall =
        "glslangValidator -V " +
        std::string(shaderDirectory) + _pathes[data._shaderNumber].string() +
        " -S " + stage + " -D" + define + " -o " +
        spvFile(data);

    // std::cout << syscall << std::endl;
    system(syscall.c_str());
}

Shader ShaderFactory::operator[](const ShaderData& data) {
    return get(data);
}
