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

module;
/**
 * @brief Global Module Fragment - contains preprocesor derectives
 * to support header units, currently compilers dont support 
 * importing header unit 
 *
 */
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cstring>
#include "VkGetter.cppm"

#include <csignal>

export module Vk.Instance;

/**
 * @brief import dependencies
 * 
 * !!!------------------ATENTION---------------------!!!
 * !!!ALL DEPENDENCIES MUST BE PRECOMPILED EARLY THEN THIS
 * 
 */
import App.Settings;

/**
 * @class Instance
 * 
 */
export class Instance {
private:
    VkInstance                              _instance;

public:
    /**
     * @brief capture VkInstance object
     * 
     */
    Instance();

    /**
     * @brief release VkINstance object
     * 
     */
    ~Instance();

    /**
     * @brief all copy/move operation forbidden
     * 
     */
    Instance(const Instance&) =delete;
    Instance& operator =(const Instance&) =delete;

    /**
     * @brief provides implicit conversion 
     * to native class very usefull in native context
     * 
     * @return VkInstance 
     */
    operator VkInstance();

private:
    /**
     * @brief help method for configuring instance
     * 
     */
    void setup();

    /**
     * @brief verificate required Vk prperties & extension support (only in debug mode)
     * @return reqiured - inout colection of data
     */
    void checkExtensionSupport(std::vector<const char*>& required);
    void checkLayerSupport(std::vector<const char*>& required);
};



/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
Instance::Instance() {
    setup();
}

Instance::~Instance() {
    vkDestroyInstance(_instance, nullptr);
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

    if (auto res = vkCreateInstance(&instanceData, nullptr, &_instance); res != VK_SUCCESS) {
        // I really need logger
        // Handle(res);
        std::raise(SIGTERM);
    }
}

Instance::operator VkInstance() {
    return _instance;
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
        std::vector<VkExtensionProperties> extensions;
        VkGet<vkEnumerateInstanceExtensionProperties>(extensions, nullptr);

        for(uint32_t i = 0; i < requiredExtCount; ++i) {
            bool hasExtension = false;
            for(const auto& vkExt: extensions) {
                hasExtension += !strcmp(vkExt.extensionName, required[i]);
            }
            if(!hasExtension) {
                // Handle(ext[i])
                // std::cout << ext[i] << std::endl;
                std::raise(SIGTERM);
            }
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

    for(auto i =0; i < required.size(); ++i) {
        bool hasLayer =false;
        for(auto avalibleLayer: availableLayers) {
            hasLayer += !strcmp(avalibleLayer.layerName, required[i]);
        }
        if(!hasLayer) {
            // Handle(layer[i])
            std::raise(SIGTERM);
        }
    }
}