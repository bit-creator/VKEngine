/**
 * @file VkInstance.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides abstraction above VkInstance
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.Instance;

export import App.NativeWrapper;

import <vector>;
import <string>;
import <memory>;

import Vulkan;
import GLFW;

import App.Settings;

import Vk.Extensions;
import Vk.Checker;
import Vk.Getter;

export struct Instance: public 
    vk::NativeWrapper < VkInstance > {
    Instance();

private:
    void setup();

    /**
     * @brief verificate required Vk prperties & extension support (only in debug mode)
     * @return reqiured - inout colection of data
     */
    void checkExtensionSupport(std::vector<const char*>& required);
    void checkLayerSupport(std::vector<const char*>& required);
}; // Instance

Instance::Instance():
    Internal([](value_type i) {
        vkDestroyInstance(i, nullptr);
    }) {
    setup();
}

void Instance::setup() {
    VkApplicationInfo appData{};
    VkInstanceCreateInfo instanceData{};

    std::vector<const char*> requiredLayers;
    std::vector<const char*> requiredExtension;

    checkExtensionSupport(requiredExtension);

    if constexpr (buildType == BuildType::Debug) {
        requiredExtension.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        checkLayerSupport(requiredLayers);  
    }

    appData.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appData.pApplicationName   = "VKEngine";
    appData.applicationVersion = VK_MAKE_VERSION(version.major, version.minor, version.path);
    appData.pEngineName        = "VKEngine";
    appData.engineVersion      = VK_MAKE_VERSION(version.major, version.minor, version.path);
    appData.apiVersion         = VK_API_VERSION_1_0;

    instanceData.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceData.pApplicationInfo        = &appData;
    instanceData.enabledLayerCount       =(uint32_t)requiredLayers.size();
    instanceData.ppEnabledLayerNames     =requiredLayers.data();
    instanceData.enabledExtensionCount   = (uint32_t)requiredExtension.size();
    instanceData.ppEnabledExtensionNames = requiredExtension.data();

    VkCreate<vkCreateInstance>(&instanceData, nullptr, &_native);
}

void Instance::checkExtensionSupport(std::vector<const char*>& required) {
    uint32_t requiredExtCount = 0;
    const char** ext;
    ext = glfwGetRequiredInstanceExtensions(&requiredExtCount);
    required.reserve(requiredExtCount);
    for(uint32_t i = 0; i < requiredExtCount; ++i) {
        required.push_back(ext[i]);
    }
    
    if constexpr (buildType == BuildType::Debug) {
        if(!Extensions<VkInstance>(_native).isSupported(required)) {
        
        }
    } 
}

void Instance::checkLayerSupport(std::vector<const char*>& required) {
    required = {
        "VK_LAYER_MESA_device_select",
        "VK_LAYER_MESA_overlay",
        "VK_LAYER_LUNARG_standard_validation",
        "VK_LAYER_KHRONOS_validation"
    };

    std::vector<VkLayerProperties> availableLayers;
    VkGet<vkEnumerateInstanceLayerProperties>(availableLayers);

    for(size_t i =0; i < required.size(); ++i) {
        bool hasLayer =false;
        for(auto avalibleLayer: availableLayers) {
            hasLayer += !strcmp(avalibleLayer.layerName, required[i]);
        }
        if(!hasLayer) {
        }
    }
}