/**
 * @file VkSemafore.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides logics for primitive sync
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.Semaphore;

import <iostream>;

export import App.NativeWrapper;

import Vulkan;
import Vk.LogicalDevice;

export class Semaphore:
    public NativeWrapper<VkSemaphore, Semaphore> {
private:
    LogicalDevice::const_pointer                _ld;

public:
    Semaphore(LogicalDevice::const_pointer device);
    ~Semaphore();
};



Semaphore::Semaphore(LogicalDevice::const_pointer device): _ld(device) {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(*device, &semaphoreInfo, nullptr, &_native) != VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphores!");
    }
}

Semaphore::~Semaphore() {
    vkDestroySemaphore(*_ld, _native, nullptr);
}

