/**
 * @file VkQueuePool.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides abstraction above Vulkan Queue families
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.QueuePool;

import Vulkan;

import <optional>;
import <vector>;
import <memory>;

import Vk.PhysicalDevice;
import Vk.WindowSurface;
import Vk.Getter;
import Vk.Queue;

export enum class QueueType {
    Present = 0,
    Graphics = 1,
    Compute = 2,
    Transfer = 3,
    SparseBinding = 4,
    Protected = 5,
};

/**
 * @brief Generally NUM_OF_QUEUE shows size of previous enum
 * 
 */
export inline constexpr uint32_t NUM_OF_QUEUE =6;

/**
 * @class QueuePool
 * @brief provide interface for work with different type of queue famelies
 * 
 * @warning QueuePool after construction knows only suported type of queues
 * but dont know descriptor for each, Queues descriptors sets in LogicalDevice
 * constructor, be sensative and check component method .isComplete() for 
 * undestanding descriptor status  
 * 
 */
export class QueuePool {
public:
    using pointer   = std::shared_ptr<QueuePool>;
    using reference = std::weak_ptr<QueuePool>;
    using const_pointer   = std::shared_ptr<const QueuePool>;
    using const_reference = std::weak_ptr<const QueuePool>;
    
private:
    std::vector<Queue>          _descriptors;
    uint32_t                    _numOfQueues;

public:
    /**
     * @brief set available all supported Queues, but not sets descriptors for each
     * 
     */
    QueuePool(PhysicalDevice::const_pointer phys, WindowSurface::const_pointer surf);

    /**
     * @param tp - type of queue
     * @return uint32_t index of queue by type
     * 
     */
    uint32_t getQueueIndex(QueueType type) const;

    /**
     * @param type - type of queue
     * @return true if queue available
     * @return false if not
     * 
     */
    bool isQueueAvailable(QueueType type) const;

    /**
     * @return uint32_t number of available queues 
     * 
     */
    uint32_t numOfAvailableQueue() const;

    /**
     * @brief set to @param indices all available indecies
     * 
     */
    void getAvailaibleIndex(std::vector<uint32_t>& indices) const;

    /**
     * @brief set descriptors for q ueues 
     * 
     */
    void setupDescriptors(const VkDevice& device);

    /**
     * @brief operator [] by queue type
     * @param type - type of queue
     * 
     */
    Queue& operator[](QueueType type);  
    const Queue& operator[](QueueType type) const;

private:
    uint32_t typeToBit(QueueType type);
    QueueType bitToType(uint32_t flag);
}; // QueuePool

/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
QueuePool::QueuePool(PhysicalDevice::const_pointer phys, WindowSurface::const_pointer surf) 
    : _numOfQueues(0) {  
    _descriptors.resize(NUM_OF_QUEUE);
    std::vector<VkQueueFamilyProperties> queueFamilies;
    VkGet<vkGetPhysicalDeviceQueueFamilyProperties>(queueFamilies, phys->get());

    uint32_t index =0;
    for(auto q: queueFamilies) {
        QueueType type = bitToType(q.queueFlags);
        if (isQueueAvailable(type)) continue;
        else {
            _descriptors[(uint32_t)type].setIndex(index);
            ++_numOfQueues;
        }
    }

    VkBool32 presentSupport = false;
    uint32_t queueIndex =0;
    vkGetPhysicalDeviceSurfaceSupportKHR(phys->get(), queueIndex, surf->get(), &presentSupport);
    
    if(!presentSupport) throw std::runtime_error("Device doesnt support presentation");

    if (!isQueueAvailable(QueueType::Present)) {
        _descriptors[(uint32_t)QueueType::Present].setIndex(queueIndex);
        ++_numOfQueues;
    }

}

uint32_t QueuePool::getQueueIndex(QueueType type) const {
    if(isQueueAvailable(type)) {
        return _descriptors[(uint32_t)type].getIndex();
    } else {
        return npos;
    }
}
    
uint32_t QueuePool::numOfAvailableQueue() const {
    return _numOfQueues;
}

bool QueuePool::isQueueAvailable(QueueType type) const {
    return _descriptors[(uint32_t)type].isSupported();
}

void QueuePool::setupDescriptors(const VkDevice& device) {
    for(auto& descr: _descriptors) {
        if(descr.isSupported()) descr.setupDescriptor(device);
    }
}

void QueuePool::getAvailaibleIndex(std::vector<uint32_t>& indices) const {
    for(const auto& descr: _descriptors) {
        if(descr.isSupported()) indices.push_back(descr.getIndex());
    }
    // its not most effective way to remove a duplicates, but for small arrays thats OK;
    indices.erase ( 
        unique(indices.begin(), indices.end()), 
        indices.end() 
    );
}

Queue& QueuePool::operator[](QueueType type) {
    return _descriptors[(uint32_t)type];
}

const Queue& QueuePool::operator[](QueueType type) const {
    return _descriptors[(uint32_t)type];
}

uint32_t QueuePool::typeToBit(QueueType type) {
    switch (type) {
        case QueueType::Present:          return 0; 
        case QueueType::Graphics:         return VK_QUEUE_GRAPHICS_BIT;  
        case QueueType::Compute:          return VK_QUEUE_COMPUTE_BIT;  
        case QueueType::Transfer:         return VK_QUEUE_TRANSFER_BIT;  
        case QueueType::SparseBinding:    return VK_QUEUE_SPARSE_BINDING_BIT;  
        case QueueType::Protected:        return VK_QUEUE_PROTECTED_BIT;  
        default:                               return VK_QUEUE_FLAG_BITS_MAX_ENUM;
    };
}; 

QueueType QueuePool::bitToType(uint32_t flag) {
    if( flag & 0)                             return QueueType::Present; 
    if( flag & VK_QUEUE_GRAPHICS_BIT)         return QueueType::Graphics;  
    if( flag & VK_QUEUE_COMPUTE_BIT)          return QueueType::Compute;  
    if( flag & VK_QUEUE_TRANSFER_BIT)         return QueueType::Transfer;  
    if( flag & VK_QUEUE_SPARSE_BINDING_BIT)   return QueueType::SparseBinding;  
    if( flag & VK_QUEUE_PROTECTED_BIT)        return QueueType::Protected;  
    throw std::runtime_error("Unknown queue bit");
}; 