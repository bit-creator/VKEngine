export module Vk.PipelineCache;

export import App.NativeWrapper;
export import Vk.Checker;

import Vulkan;
export import Vk.LogicalDevice;

import <vector>;

using Cache = std::vector<std::byte>;

export struct PipelineCache:
        public vk::NativeWrapper <VkPipelineCache> {
    PipelineCache(LogicalDevice device);
    PipelineCache(LogicalDevice device, const Cache& cache);
};

PipelineCache::PipelineCache(LogicalDevice device):
        Internal([&](value_type pc){vkDestroyPipelineCache(device, pc, nullptr);}) {
    VkPipelineCacheCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.initialDataSize = 0;
    info.pInitialData = nullptr;

    VkCreate<vkCreatePipelineCache>(device, &info, nullptr, &_native);
}

PipelineCache::PipelineCache(LogicalDevice device, const Cache& cache):
        Internal([&](value_type pc){vkDestroyPipelineCache(device, pc, nullptr);}) {
    VkPipelineCacheCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.initialDataSize = cache.size();
    info.pInitialData = cache.data();

    VkCreate<vkCreatePipelineCache>(device, &info, nullptr, &_native);
}