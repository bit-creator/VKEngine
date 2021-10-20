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

export module Vk.LogicalDevice;

export import App.NativeWrapper;

import Vulkan;

import <vector>;

import Vk.PhysicalDevice;
import Vk.QueuePool;
import Vk.Checker;

/**
 * @class LogicalDevice
 * 
 * @warning copy/move constructors tagged =delete becouse I actualy 
 * dont know how correct copy LogicDevice
 */
export class LogicalDevice:
    public NativeWrapper<VkDevice, LogicalDevice> {

public:
    /**
     * @brief capture VkLogicalDevice object
     * 
     * @param device - ref to PhysDevice
     * @param queues - ref to QueuePool, need for set queues descriptors to pool
     * 
     */
    LogicalDevice(PhysicalDevice::const_pointer device, QueuePool::pointer queues);

    /**
     * @brief release capture objects
     * 
     */
    ~LogicalDevice();
}; // LogicalDevice

/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
LogicalDevice::LogicalDevice(PhysicalDevice::const_pointer device, QueuePool::pointer queues) {
    float queuePriority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queueInfos;
    std::vector<uint32_t> avaialableQueue;
    queues->getAvailaibleIndex(avaialableQueue);

    queueInfos.resize(avaialableQueue.size());

    uint32_t enumerator =0;
    for(auto& info: queueInfos) {
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.queueFamilyIndex = avaialableQueue[enumerator];
        info.queueCount = 1;
        info.pQueuePriorities = &queuePriority;
        ++enumerator;
    }

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device->get(), &deviceFeatures);

    const auto ext = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueInfos.data();
    createInfo.queueCreateInfoCount = (uint32_t)queueInfos.size();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = &ext;

    VkCreate<vkCreateDevice>(device->get(), &createInfo, nullptr, &_native);

    queues->setupDescriptors(_native);
}

LogicalDevice::~LogicalDevice() {
    vkDestroyDevice(_native, nullptr);
}