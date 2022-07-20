export module Vk.CommandPool;

import Vulkan;
import Vk.LogicalDevice;

export {
struct CommandPool:
        public vk::NativeWrapper<VkCommandPool> {
    CommandPool(LogicalDevice device, QueueType type);
    static CommandPool invalid() {return CommandPool{};}
protected:
    CommandPool(){};
}; // CommandPool

struct DrawCmdPool: public CommandPool {
    DrawCmdPool(LogicalDevice device);
}; // DrawCmdPool

struct TransferCmdPool: public CommandPool {
    TransferCmdPool(LogicalDevice device);
    static TransferCmdPool invalid() {return TransferCmdPool{};}
private:
    TransferCmdPool(){};
}; // TransferCmdPool
}; // export

CommandPool::CommandPool(LogicalDevice device, QueueType type):
        Internal([&](value_type cp){ vkDestroyCommandPool(device, cp, nullptr); }) {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = device.queues[type].getIndex();
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &_native) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

DrawCmdPool::DrawCmdPool(LogicalDevice device): CommandPool(device, QueueType::Graphics) {
}

TransferCmdPool::TransferCmdPool(LogicalDevice device): CommandPool(device, QueueType::Transfer) {
}