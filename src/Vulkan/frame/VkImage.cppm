export module Vk.Image;

import Image;
import Vulkan;
import <filesystem>;
import <iostream>;

import Vk.VertexBuffer;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;
import Vk.CommandBuffer;
import Vk.Memory;
import Vk.Checker;

namespace fs = std::filesystem;

export enum class ImageLayout {
    Undefined   =VK_IMAGE_LAYOUT_UNDEFINED,
    Destination =VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    Read        =VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
};

export class Image:
    public vk::NativeWrapper<VkImage> {
private:
    Memory          _mem;
    ImageLayout     _layout;
    CommandBuffer   _buff;
    Queue           _queue;
    VkFormat        _format;

public:
    Image(fs::path path, LogicalDevice device, PhysicalDevice phys, CommandBuffer buff);

    VkFormat format() { return _format; }
    VkImageLayout layout() { return (VkImageLayout)_layout; };

private:
    void load(fs::path path, LogicalDevice device, PhysicalDevice phys);
    void switchLayout(ImageLayout layout);
};

Image::Image(fs::path path, LogicalDevice device, PhysicalDevice phys, CommandBuffer buff)
    : Internal([&](value_type img){ vkDestroyImage(device, img, nullptr); })
    , _mem(device)
    , _buff(buff) {
    _queue = device.queues.transfer;
    load(path, device, phys);
}

void Image::load(fs::path path, LogicalDevice device, PhysicalDevice phys) {
    int width, height, nrChannels;

	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if(data == NULL) std::cout << "Couldn\'t load image" << std::endl;

    StagingBuffer buff(device);

    buff.allocateAndFill(device, phys, width * height * nrChannels, data);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = nrChannels == 4? VK_FORMAT_R8G8B8A8_SRGB: VK_FORMAT_R8G8B8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    _format = imageInfo.format;

    if (vkCreateImage(device, &imageInfo, nullptr, &_native) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, _native, &memRequirements);

    uint32_t typeFilter = memRequirements.memoryTypeBits;
    VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = _mem.findMemoryType(typeFilter, properties, phys);
    
    VkCreate<vkAllocateMemory>(device, &allocInfo, nullptr, &_mem.native());

    vkBindImageMemory(device, _native, _mem, 0);

    switchLayout(ImageLayout::Destination);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        (uint32_t)width,
        (uint32_t)height,
        1
    };

    _buff.recordAndSubmit(_queue, [&](CommandBuffer cmd) {
        vkCmdCopyBufferToImage(
            cmd,
            buff,
            _native,
            layout(),
            1,
            &region
        );
    });

    switchLayout(ImageLayout::Read);
}

void Image::switchLayout(ImageLayout layout) {
    if(layout == _layout) return;

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = (VkImageLayout)_layout;
    barrier.newLayout = (VkImageLayout)layout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = _native;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (_layout == ImageLayout::Undefined && layout == ImageLayout::Destination) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (_layout == ImageLayout::Destination && layout == ImageLayout::Read) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    _buff.recordAndSubmit(_queue, [&](CommandBuffer cmd) {
        vkCmdPipelineBarrier(
            cmd,
            sourceStage, destinationStage,
            VK_DEPENDENCY_BY_REGION_BIT,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );
    });

    _layout = layout;
}