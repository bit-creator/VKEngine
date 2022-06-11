/**
 * @file VkPhysDevice.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides abstraction above Vulkan Physical Device
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.PhysicalDevice;

export import App.NativeWrapper;

import <vector>;
import <iostream>;

import Vulkan;
import GLFW;

import Vk.Extensions;
import Vk.Getter;

/**
 * @class PhysicalDevice
 * @brief provides abstraction above VkPhysicalDevice and 
 * also have a abstraction above Queue indecies search
 * 
 */
export class PhysicalDevice:
    public vk::NativeWrapper < VkPhysicalDevice > {
public:
    PhysicalDevice(VkInstance instance);

    static PhysicalDevice invalid() {
        return PhysicalDevice{};
    }

    ~PhysicalDevice() =default;

private:
    /**
     * @brief select concreate device by criteria
     * 
     * @param devices inout colection by native device objects
     */
    void select(std::vector<VkPhysicalDevice>& devices);

    /**
     * @param device tested device
     * @return true if correspond to criteria
     * @return false if no
     */
    bool isCorrectProperty(VkPhysicalDevice device);

    /**
     * @param device tested device
     * @return true if correspond to criteria
     * @return false if no
     */
    bool isCorrercFeatures(VkPhysicalDevice device);

    PhysicalDevice() =default;
}; // PhysicalDevice

PhysicalDevice::PhysicalDevice(VkInstance instance): Internal() {
    native(VK_NULL_HANDLE);
    std::vector<VkPhysicalDevice> devices;
    
    VkGet<vkEnumeratePhysicalDevices>(devices, instance);
    
    select(devices);
    
    native() ?: throw std::runtime_error("Physical device not found"); 
}

void PhysicalDevice::select(std::vector<VkPhysicalDevice>& devices) {
    for (const auto& device : devices) {
        if ( isCorrectProperty(device) &&
             isCorrercFeatures(device) ){
            _native = device;
            break;
        }
    }

    if (_native == VK_NULL_HANDLE) {
        throw std::runtime_error("PhysicalDevice not suitable");
    }
}

bool PhysicalDevice::isCorrectProperty( [[maybe_unused]] VkPhysicalDevice device) {
    // VkPhysicalDeviceFeatures2 pFeatures;
    // VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT pl;
    // pl.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GRAPHICS_PIPELINE_LIBRARY_FEATURES_EXT;
    // pl.pNext = nullptr;
    // pl.graphicsPipelineLibrary = true;

    // pFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    // pFeatures.pNext = &pl;

    // vkGetPhysicalDeviceFeatures2(device, &pFeatures);
    
    return true;
}

bool PhysicalDevice::isCorrercFeatures( [[maybe_unused]] VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    std::cout << deviceProperties.deviceName << std::endl;

    // // return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
    //        deviceFeatures.geometryShader;
    return true;
}