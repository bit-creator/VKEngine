export module Vk.Memory;

import Vulkan;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;
import Vk.Checker;

// must be allocated manualy

export class Memory:
    public vk::NativeWrapper<VkDeviceMemory> {
public:
    Memory(LogicalDevice ld);
};

Memory::Memory(LogicalDevice ld):
    Internal([&](value_type mem){ vkFreeMemory(ld, mem, nullptr); }){
}