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
export import Vk.Queue;

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
    PresentQueue                                present;
    GraphicsQueue                               graphic;
    ComputeQueue                                compute;
    TransferQueue                               transfer;
    SparseBindingQueue                          sparseBinding;
    ProtectedQueue                              protectedQ;

public:
    QueuePool(PhysicalDevice phys, WindowSurface surf);

    // uint32_t getQueueIndex(QueueType type) const;

    // bool isQueueAvailable(QueueType type) const;

    // uint32_t numOfAvailableQueue() const;

    // void getAvailaibleIndex(std::vector<uint32_t>& indices) const;

    void setupDescriptors(const VkDevice& device);

    Queue& get(QueueType type);
    Queue& get(uint32_t flag);

    Queue& operator[](QueueType type);  
    // const Queue& operator[](QueueType type) const;
    
    QueueType bitToType(uint32_t flag);
}; // QueuePool
// #include <vulkan/vulkan_core.h>
QueuePool::QueuePool(PhysicalDevice phys, WindowSurface surf) {  
    std::vector<VkQueueFamilyProperties> queueFamilies;
    VkGet<vkGetPhysicalDeviceQueueFamilyProperties>(queueFamilies, phys);

    // uint32_t index =0;
    // for(auto [ flag, count, tsbits, transferGranularuty ]: queueFamilies) {
    //     std::cout << queueFamilies.size() << std::endl;
    //     std::cout << VK_QUEUE_GRAPHICS_BIT << std::endl;
    //     // if   (get(flag).isSupported()) continue;
    //     // else 
    //      get(flag).setIndex(index);
    //     ++index;
    // }

    graphic.setIndex(0);

    VkBool32 presentSupport = false;
    // uint32_t queueIndex =0;
    vkGetPhysicalDeviceSurfaceSupportKHR(phys, graphic.getIndex(), surf, &presentSupport);
    
    if(!presentSupport) throw std::runtime_error("Device doesnt support presentation");

    present.setIndex(graphic.getIndex());
}

// uint32_t QueuePool::getQueueIndex(QueueType type) const {
//     // if(isQueueAvailable(type)) {
//     //     return _descriptors[(uint32_t)type].getIndex();
//     // } else {
//     //     return npos;
//     // }
// }
    
// uint32_t QueuePool::numOfAvailableQueue() const {
//     // return _numOfQueues;
// }

// bool QueuePool::isQueueAvailable(QueueType type) const {
//     // return _descriptors[(uint32_t)type].isSupported();
// }

void QueuePool::setupDescriptors(const VkDevice& device) {
    graphic.setupDescriptor(device);
    compute.setupDescriptor(device);
    transfer.setupDescriptor(device);
    sparseBinding.setupDescriptor(device);
    protectedQ.setupDescriptor(device);
    present.setupDescriptor(device);
}

// void QueuePool::getAvailaibleIndex(std::vector<uint32_t>& indices) const {
//     for(const auto& descr: _descriptors) {
//         if(descr.isSupported()) indices.push_back(descr.getIndex());
//     }
//     // its not most effective way to remove a duplicates, but for small arrays thats OK;
//     indices.erase ( 
//         unique(indices.begin(), indices.end()), 
//         indices.end() 
//     );
// }


Queue& QueuePool::operator[](QueueType type) {
    return get(type);
}

// const Queue& QueuePool::operator[](QueueType type) const {
//     return get(type);
// }

Queue& QueuePool::get(QueueType type) {
    switch (type) {
        case QueueType::Graphics:         return graphic;  
        case QueueType::Compute:          return compute;  
        case QueueType::Transfer:         return transfer;  
        case QueueType::SparseBinding:    return sparseBinding;  
        case QueueType::Protected:        return protectedQ;  
        default:                          return present;
    }
}

Queue& QueuePool::get(uint32_t flag) {
    switch (flag) {
        case VK_QUEUE_GRAPHICS_BIT:         return graphic;  
        case VK_QUEUE_COMPUTE_BIT:          return compute;  
        case VK_QUEUE_TRANSFER_BIT:         return transfer;  
        case VK_QUEUE_SPARSE_BINDING_BIT:   return sparseBinding;  
        case VK_QUEUE_PROTECTED_BIT:        return protectedQ;  
        default:                            return present;
    }
}

// uint32_t QueuePool::typeToBit(QueueType type) {
//     switch (type) {
//         case QueueType::Present:          return 0; 
//         case QueueType::Graphics:         return VK_QUEUE_GRAPHICS_BIT;  
//         case QueueType::Compute:          return VK_QUEUE_COMPUTE_BIT;  
//         case QueueType::Transfer:         return VK_QUEUE_TRANSFER_BIT;  
//         case QueueType::SparseBinding:    return VK_QUEUE_SPARSE_BINDING_BIT;  
//         case QueueType::Protected:        return VK_QUEUE_PROTECTED_BIT;  
//         default:                               return VK_QUEUE_FLAG_BITS_MAX_ENUM;
//     };
// }; 

QueueType QueuePool::bitToType(uint32_t flag) {
    if( flag & 0)                             return QueueType::Present; 
    if( flag & VK_QUEUE_GRAPHICS_BIT)         return QueueType::Graphics;  
    if( flag & VK_QUEUE_COMPUTE_BIT)          return QueueType::Compute;  
    if( flag & VK_QUEUE_TRANSFER_BIT)         return QueueType::Transfer;  
    if( flag & VK_QUEUE_SPARSE_BINDING_BIT)   return QueueType::SparseBinding;  
    if( flag & VK_QUEUE_PROTECTED_BIT)        return QueueType::Protected;  
    throw std::runtime_error("Unknown queue bit");
}; 