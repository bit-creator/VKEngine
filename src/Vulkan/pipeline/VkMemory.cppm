export module Vk.Memory;

import Vulkan;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;
import Vk.Checker;

// must be allocated manualy

// to-do:/ impliment MapGuard
// to-do:/ impliment Allocator
export class Memory:
        public vk::NativeWrapper<VkDeviceMemory> {
    LogicalDevice _ld;
public:
    Memory(LogicalDevice ld);
    void load(void* data, size_t size); 
};


// to-do:/ something magic in this... I recieve crash in staging buffer when vkFreeMemory is called;
Memory::Memory(LogicalDevice ld):
    Internal([&](value_type mem){ /*vkFreeMemory(ld, mem, nullptr);*/ }),
    _ld(ld){
}

void Memory::load(void* data, size_t size) {
    void* internal_data;
    vkMapMemory(_ld, _native, 0, size, 0, &internal_data);
        memcpy(internal_data, data, size);
    vkUnmapMemory(_ld, _native);
}