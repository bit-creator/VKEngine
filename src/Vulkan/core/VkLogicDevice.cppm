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
export import Vk.QueuePool;
import Vk.WindowSurface;
import Vk.Checker;

export struct LogicalDevice: public 
    vk::NativeWrapper < VkDevice > {
    /**
     * @param queues - ref to QueuePool, need for set queues descriptors to pool
     */
    LogicalDevice(PhysicalDevice device, WindowSurface surf);

    QueuePool   queues;

    static LogicalDevice invalid() {
        return LogicalDevice{};
    }

private:
    LogicalDevice(): Internal([](const value_type& l){ vkDestroyDevice(l, nullptr); }) {};
}; // LogicalDevice

LogicalDevice::LogicalDevice(PhysicalDevice device, WindowSurface surf)
    : Internal([](const value_type& l){ vkDestroyDevice(l, nullptr); })
    , queues(QueuePool(device, surf)) 
    {
    float queuePriority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queueInfos;
    // std::vector<uint32_t> avaialableQueue;
    // queues.getAvailaibleIndex(avaialableQueue);

    // queueInfos.resize();
    queueInfos.push_back({
                VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                nullptr, 0, 0, 1, &queuePriority
            });
        
    // for(uint32_t i = 0; i < NUM_OF_QUEUE; ++i) {
    //     if (auto index = queues[(QueueType)i].getIndex(); index != npos)
    //         queueInfos.push_back({
    //             VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    //             nullptr, 0, index, 1, &queuePriority
    //         });
        // info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        // info.queueFamilyIndex = avaialableQueue[enumerator];
        // info.queueCount = 1;
        // info.pQueuePriorities = &queuePriority;
        // ++enumerator;
    // }
    // std::cout << queueInfos.size() << std::endl;
    //     

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // const auto ext = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    std::vector<const char*> extensions;
    extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    extensions.push_back(VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME);
    extensions.push_back(VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME);

    for(auto& ext: extensions) {
        std::cout << ext << std::endl;
    }

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueInfos.data();
    createInfo.queueCreateInfoCount = (uint32_t)queueInfos.size();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();


    VkCreate<vkCreateDevice>(device, &createInfo, nullptr, &_native);

    queues.setupDescriptors(_native);
}