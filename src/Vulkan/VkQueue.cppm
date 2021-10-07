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

import Vulkan;

import Vk.PhysicalDevice;
import Vk.LogicalDevice;

/**
 * @class Queue
 * @brief base abstract class, for construction
 * use a derived class that provides queue 
 * construction with a specific queue type
 * 
 */
export class Queue {
private:
    VkQueue                 _queue;
    QueueType               _type;
protected:
    /**
     * @brief get a descriptor to queue object by type
     * 
     * @param tp - type of needed queue
     * @param phys - ref to physical device
     * @param log - ref to logical device
     * 
     */
    Queue(QueueType tp, PhysicalDevice& phys, LogicalDevice& log);

public:
    /**
     * @brief Destroy the Queue object
     * tagged =default becouse all Queue desriptors
     * releases with destroying logical device
     * 
     */
    ~Queue() =default;

    /**
     * @brief get a type of queue
     * 
     * @return QueueType 
     */
    QueueType type();

    /**
     * @brief provides implicit conversion 
     * to native class very usefull in native context
     * 
     * @return VkQueue 
     */
    operator VkQueue();

    /**
     * @brief only move operation alloved
     * 
     */
    Queue(const Queue&) =delete;
    Queue& operator =(const Queue&) =delete;

    Queue(Queue&&) =default;
    Queue& operator =(Queue&&) =default;
};

/**
 * @class GraphicQueue
 * @brief provides the creation of a queue with
 *  a specific type =QueueType::Graphics
 * 
 */
export class GraphicQueue: public Queue {
public: GraphicQueue(PhysicalDevice& phys, LogicalDevice& log);
};
// etc...


/********************************************/
/***************IMPLIMENTATION***************/
/********************************************/
Queue::Queue(QueueType tp, PhysicalDevice& phys, LogicalDevice& log) 
    : _type(tp)
{
    vkGetDeviceQueue(log, phys.getQueueIndex(tp), 0, &_queue);
}

QueueType Queue::type() {
    return _type;
}

Queue::operator VkQueue() {
    return _queue;
}

GraphicQueue::GraphicQueue(PhysicalDevice& phys, LogicalDevice& log)
    : Queue(QueueType::Graphics, phys, log) 
{  }
