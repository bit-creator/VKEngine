export module Vk.Fence;

import Vulkan;
import App.NativeWrapper;
import Vk.LogicalDevice;
import Vk.Checker;

export struct Fence: public vk::NativeWrapper<VkFence> {
    Fence(LogicalDevice ld);
};

Fence::Fence(LogicalDevice ld):
    Internal([&](value_type f){ vkDestroyFence(ld, f, nullptr); }) {
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkCreate<vkCreateFence>(ld, &fenceInfo, nullptr, &_native);
}