/**
 * @file VkQueue.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides abstraction above Vulkan queues 
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.Queue;

export import App.NativeWrapper;

import <optional>;
import <limits>;

import Vulkan;

export inline constexpr uint32_t npos = std::numeric_limits<uint32_t>::max();

/**
 * @class Queue
 * @brief default constructible/destructible
 * 
 */
export class Queue: 
    public NativeWrapper<VkQueue, Queue> {
private:
    std::optional<uint32_t> _index;

public:
    Queue() =default;
    ~Queue() =default;

    /**
     * @warning actually because Queue default 
     * constructible/destructible, but be 
     * sensitive every copying operation 
     * need external set for descriptor
     * 
     */
    Queue(const Queue& o);

    /**
     * @brief setter/getter for _index;
     * 
     */
    void setIndex(uint32_t index);
    uint32_t getIndex() const;

    /**
     * @brief queue status checkers
     * 
     */
    bool isCompleted() const;
    bool isSupported() const;

    /**
     * @brief setups descriptor, requires completed
     * LogicalDevice
     */
    void setupDescriptor(const VkDevice& device);
}; // Queue


/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
Queue::Queue(const Queue& o): _index(o._index) {

}

void Queue::setIndex(uint32_t index) {
    _index = index;
}

bool Queue::isCompleted() const {
    return _native != VK_NULL_HANDLE;
}

bool Queue::isSupported() const {
    return _index.has_value();
}

uint32_t Queue::getIndex() const {
    return _index.value_or(npos);
}

void Queue::setupDescriptor(const VkDevice& device) {
    if (isSupported()) vkGetDeviceQueue(device, _index.value(), 0, &_native);
}