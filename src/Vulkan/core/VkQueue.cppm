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

export import <optional>;
import <limits>;

import Vulkan;

export inline constexpr uint32_t npos = std::numeric_limits<uint32_t>::max();

export enum class QueueType {
    Present         = 0,
    Graphics        = 1,
    Compute         = 2,
    Transfer        = 3,
    SparseBinding   = 4,
    Protected       = 5,
};

/**
 * @class Queue
 * @brief default constructible/destructible
 * 
 */
export {
class Queue: 
    public vk::NativeWrapper < VkQueue > {
private:
    std::optional<uint32_t>         _index;

public:
    void setIndex(uint32_t index);
    uint32_t getIndex() const;

    bool isCompleted() const;
    bool isSupported() const;

    void setupDescriptor(const VkDevice& device);
}; // Queue

template < QueueType type >
struct QueueType2BitImpl;

template <> struct QueueType2BitImpl < QueueType::Present > {
    static inline uint32_t QueueTypeBit = 0; 
};

template <> struct QueueType2BitImpl < QueueType::Graphics > {
    static inline uint32_t QueueTypeBit = VK_QUEUE_GRAPHICS_BIT; 
};

template <> struct QueueType2BitImpl < QueueType::Compute > {
    static inline uint32_t QueueTypeBit = VK_QUEUE_COMPUTE_BIT;
};

template <> struct QueueType2BitImpl < QueueType::Transfer > {
    static inline uint32_t QueueTypeBit = VK_QUEUE_TRANSFER_BIT;
};

template <> struct QueueType2BitImpl < QueueType::SparseBinding > {
    static inline uint32_t QueueTypeBit = VK_QUEUE_SPARSE_BINDING_BIT;
};

template <> struct QueueType2BitImpl < QueueType::Protected > {
    static inline uint32_t QueueTypeBit = VK_QUEUE_PROTECTED_BIT;
};

template < QueueType type >
inline constexpr uint32_t QueueType2Bit = QueueType2BitImpl<type>::QueueTypeBit;

template < QueueType type >
class ConcreateQueue: public Queue { 
    constexpr uint32_t  getBit()  const noexcept;
    constexpr QueueType getType() const noexcept;
};

using PresentQueue       = ConcreateQueue < QueueType::Present       >;
using GraphicsQueue      = ConcreateQueue < QueueType::Graphics      >;
using ComputeQueue       = ConcreateQueue < QueueType::Compute       >;
using TransferQueue      = ConcreateQueue < QueueType::Transfer      >;
using SparseBindingQueue = ConcreateQueue < QueueType::SparseBinding >;
using ProtectedQueue     = ConcreateQueue < QueueType::Protected     >;

};

template < QueueType type >
    constexpr uint32_t ConcreateQueue<type>::getBit() const noexcept {
        return QueueType2Bit<type>;
}

template < QueueType type >
    constexpr QueueType ConcreateQueue<type>::getType() const noexcept {
        return type;
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