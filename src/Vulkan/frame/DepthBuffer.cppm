export module Vk.DepthBuffer;

import Vulkan;
import App.NativeWrapper;
import Vk.Image;
import Vk.ImageView;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;
import Vk.CommandBuffer;
import Vk.Memory;
import Vk.Checker;

import <vector>;

export struct DepthImage: public Image {
    DepthImage(LogicalDevice device, PhysicalDevice phys,
            CommandBuffer buff, VkExtent2D size);
    VkFormat format() { return _format; }
    bool hasStencilComponent(VkFormat format);
};

export class DepthBuffer {
    DepthImage          _image;
    ImageView           _view;

public:
    DepthBuffer(LogicalDevice device, PhysicalDevice phys,
            CommandBuffer buff, VkExtent2D size);
    
    ImageView view() { return _view; }
};

DepthImage::DepthImage(LogicalDevice device, PhysicalDevice phys,
                        CommandBuffer buff, VkExtent2D size)
    : Image(device, buff) {
    // std::vector<VkFormat> candidates = {
    //     VK_FORMAT_D32_SFLOAT_S8_UINT,
    //     VK_FORMAT_D24_UNORM_S8_UINT,
    //     VK_FORMAT_D32_SFLOAT
    // };

    // VkImageTiling        tiling     = VK_IMAGE_TILING_OPTIMAL;
    // VkFormatFeatureFlags features   = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

    // for (VkFormat format : candidates) {
    //     VkFormatProperties props;
    //     vkGetPhysicalDeviceFormatProperties(phys, format, &props);
    //     if (tiling == VK_IMAGE_TILING_LINEAR &&
    //             (props.linearTilingFeatures & features) == features) {
    //         _format = format; break;
    //     } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
    //             (props.optimalTilingFeatures & features) == features) {
    //         _format = format; break;
    //     }
    // }

    _format = VK_FORMAT_D32_SFLOAT_S8_UINT;

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width =  size.width;
    imageInfo.extent.height = size.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = _format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VkImageLayout(_layout);
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

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

    switchLayout(ImageLayout::DSOptimal);
}

bool DepthImage::hasStencilComponent(VkFormat format) {
    return _format == VK_FORMAT_D32_SFLOAT_S8_UINT || _format == VK_FORMAT_D24_UNORM_S8_UINT;
}


DepthBuffer::DepthBuffer(LogicalDevice device, PhysicalDevice phys,
                        CommandBuffer buff, VkExtent2D size)
        : _image(device, phys, buff, size)
        , _view(_image.native(), _image.format(), device,
            (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT)) {
}