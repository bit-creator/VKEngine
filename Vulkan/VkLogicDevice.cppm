/**
 * @file VkLogicDevice.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides abstraction above Vulkan Logical Device
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
#include <iostream>

export module Vk.LogicalDevice;

/**
 * @brief import dependencies
 * 
 * !!!------------------ATENTION---------------------!!!
 * !!!ALL DEPENDENCIES MUST BE PRECOMPILED EARLY THEN THIS
 * 
 */
import Vk.PhysicalDevice;

/**
 * @class LogicalDevice
 * 
 * @warning copy/move constructors tagged =delete becouse I actualy 
 * dont know how correct copy LogicDevice
 */
export class LogicalDevice {
private:
    VkDevice                    _device;

public:
    /**
     * @brief capture VkLogicalDevice object
     * 
     * @param device - ref to PhysDevice
     */
    LogicalDevice(PhysicalDevice& device);

    /**
     * @brief release capture objects
     * 
     */
    ~LogicalDevice();

    /**
     * @brief provides implicit conversion 
     * to native class very usefull in native context
     * 
     * @return VkDevice 
     */
    operator VkDevice();

    LogicalDevice(const LogicalDevice&) =delete;
    LogicalDevice& operator =(const LogicalDevice&) =delete;
};






/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
LogicalDevice::LogicalDevice(PhysicalDevice& device) {
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = device.getQueueIndex(QueueType::Graphics);
    queueCreateInfo.queueCount = device.numOfAvailableQueue();
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

    if (vkCreateDevice(device, &createInfo, nullptr, &_device) != VK_SUCCESS) {
        // LOG IT
        std::cout << "LogicDevice Creation Failed" << std::endl;
    }
}

LogicalDevice::~LogicalDevice() {
    vkDestroyDevice(_device, nullptr);
}

LogicalDevice::operator VkDevice() {
    return _device;
}