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

export struct Semaphore: public 
    vk::NativeWrapper<VkSemaphore> {
    Semaphore(LogicalDevice device);
};

Semaphore::Semaphore(LogicalDevice device):
    Internal([&](value_type s){ vkDestroySemaphore(device, s, nullptr); }) {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_native) != VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphores!");
    }
}