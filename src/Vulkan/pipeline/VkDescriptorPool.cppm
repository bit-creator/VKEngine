export module Vk.DescriptorPool;

import Vulkan;
import App.NativeWrapper;
import Vk.LogicalDevice;
import Vk.Checker;
import <vector>;
import <iostream>;

export struct DescriptorSetLayout:
    public vk::NativeWrapper<VkDescriptorSetLayout> {
    DescriptorSetLayout(LogicalDevice ld, size_t binding,
        VkDescriptorType type,VkShaderStageFlagBits stage);
};

export struct DescriptorPool:
        public vk::NativeWrapper<VkDescriptorPool> {
    DescriptorPool(LogicalDevice ld);
    VkDescriptorSet allocate();
    DescriptorSetLayout layout() { return _layout; }

private:
    LogicalDevice           _ld;
    DescriptorSetLayout     _layout;
};

DescriptorSetLayout::DescriptorSetLayout(LogicalDevice ld, size_t binding,
        VkDescriptorType type,VkShaderStageFlagBits stage):
    Internal([&](value_type l) {vkDestroyDescriptorSetLayout(ld, l, nullptr);}) {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = type;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = stage;
    uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    VkCreate<vkCreateDescriptorSetLayout>(ld, &layoutInfo, nullptr, &_native);
}

DescriptorPool::DescriptorPool(LogicalDevice ld):
        Internal([&](value_type dp){ vkDestroyDescriptorPool(ld, dp, nullptr); }), _ld(ld),
        _layout(_ld, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) {
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSize.descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = 10;

    VkCreate<vkCreateDescriptorPool>(ld, &poolInfo, nullptr, &_native);
}
   
VkDescriptorSet DescriptorPool::allocate() {
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = _native;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &_layout.native();

    VkDescriptorSet set;
    VkCreate<vkAllocateDescriptorSets>(_ld, &allocInfo, &set);
    return set;
}