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

import Vulkan;
import GLFW;

import <vector>;
import <bitset>;
import <array>;

import Vk.Extensions;
import Vk.Getter;

/**
 * @brief enumeration with a Queue types
 * asociated with a specific values
 * 
 */
export enum class QueueType {
    Graphics =0,
    Present =1
};

/**
 * @brief Generally NUM_OF_QUEUE shows size of previous enum
 * 
 */
export inline constexpr uint32_t NUM_OF_QUEUE =2;

/**
 * @brief help value for unvalid data
 * 
 */
export inline constexpr uint32_t npos = std::numeric_limits<uint32_t>::max();

/**
 * @class PhysicalDevice
 * @brief provides abstraction above VkPhysicalDevice and 
 * also have a abstraction above Queue indecies search
 * 
 */
export class PhysicalDevice {
private:
    VkPhysicalDevice                        _device;
    std::bitset<NUM_OF_QUEUE>               _availableQueues;
    std::array<uint32_t, NUM_OF_QUEUE>      _queuesIndex;

public:
    /**
     * @brief capture VkPhysicalDevice
     * 
     * @param instance 
     */
    PhysicalDevice(VkInstance instance);

    /**
     * @brief release VkPhysicalDevice and
     * destroy object
     * 
     */
    ~PhysicalDevice();

    /**
     * @brief all copy/move operation forbidden
     * 
     */
    PhysicalDevice(const PhysicalDevice&) =delete;
    PhysicalDevice& operator =(const PhysicalDevice&) =delete;

    /**
     * @param tp - type of queue
     * @return uint32_t index of queue by type
     */
    uint32_t getQueueIndex(QueueType tp);

    /**
     * @param tp - type of queue
     * @return true if queue available
     * @return false if not
     */
    bool isQueueAvailable(QueueType tp);

    /**
     * @return uint32_t number of available queues 
     */
    uint32_t numOfAvailableQueue();

    /**
     * @brief provides implicit conversion 
     * to native class very usefull in native context
     * 
     * @return VkPhysicalDevice 
     */
    operator VkPhysicalDevice();
    operator VkPhysicalDevice() const;

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

    /**
     * @brief actualizing _availableQueues & _queuesIndex data
     * 
     */
    void setupQueue();
};


/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
PhysicalDevice::PhysicalDevice(VkInstance instance) : _device(VK_NULL_HANDLE) {
    std::vector<VkPhysicalDevice> devices;
    VkGet<vkEnumeratePhysicalDevices>(devices, instance);
    select(devices);
    if (_device == VK_NULL_HANDLE) { /*Handle it*/ }
    setupQueue();
}

PhysicalDevice::~PhysicalDevice() {
}

PhysicalDevice::operator VkPhysicalDevice() {
    return _device;
}

PhysicalDevice::operator VkPhysicalDevice() const {
    return _device;
}

void PhysicalDevice::select(std::vector<VkPhysicalDevice>& devices) {
    for (const auto& device : devices) {
        if ( isCorrectProperty(device) &&
             isCorrercFeatures(device) ){
            _device = device;
            break;
        }
    }

    if (_device == VK_NULL_HANDLE) {
        // std::raise(SIGTERM);
    }
}

bool PhysicalDevice::isCorrectProperty(VkPhysicalDevice device) {
    return true;
}

bool PhysicalDevice::isCorrercFeatures(VkPhysicalDevice device) {
    return true;
}

uint32_t PhysicalDevice::getQueueIndex(QueueType tp) {
    if(isQueueAvailable(tp)) {
        return _queuesIndex[(uint32_t)tp];
    } else {
        /*handle it*/
        return npos;
    }
}
    
uint32_t PhysicalDevice::numOfAvailableQueue() {
    return _availableQueues.count();
}

bool PhysicalDevice::isQueueAvailable(QueueType tp) {
    return _availableQueues.test((uint32_t)tp);
}

void PhysicalDevice::setupQueue() {
    std::vector<VkQueueFamilyProperties> queueFamilies;
    VkGet<vkGetPhysicalDeviceQueueFamilyProperties>(queueFamilies, _device);

    auto findQueueIndex = [&queueFamilies, this](QueueType tp, VkQueueFlagBits flag) -> void {
        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & flag) {
                _queuesIndex[(uint32_t)tp] = i;
                _availableQueues[(uint32_t)tp] = true;
                break;
            }
            i++;
        }
    };

    findQueueIndex(QueueType::Graphics, VK_QUEUE_GRAPHICS_BIT);
    //...


    if(!Extensions<VkPhysicalDevice>(_device).isSupported(VK_KHR_SWAPCHAIN_EXTENSION_NAME)) {
        // std::cout << "swapchain not supported" <<std::endl;
    }

    // std::cout << _device.deviceName << std::endl;

    std::vector <VkExtensionProperties> vec;
    Extensions<VkPhysicalDevice>{_device}.getAllSupported(vec);
    for(auto v: vec) {
        // std::cout << v.extensionName <<std::endl;
    }
}