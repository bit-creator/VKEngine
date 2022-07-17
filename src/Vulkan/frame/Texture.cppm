export module App.Texture2D;

import Vk.Image;
import Vk.ImageView;
import Vk.Image.Sampler;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;
import Vk.CommandBuffer;

import <filesystem>;

namespace fs = std::filesystem;

export class Texture2D {
    Image               _image;
    ImageView           _view;
    Sampler             _sampler;

public:
    VkDescriptorSet     _set;
    Texture2D(fs::path path, LogicalDevice device, PhysicalDevice phys, CommandBuffer buff, VkDescriptorSet set);
};

Texture2D::Texture2D(fs::path path, LogicalDevice device, PhysicalDevice phys, CommandBuffer buff, VkDescriptorSet set)
        : _image(path, device, phys, buff)
        , _view(_image, _image.format(), device)
        , _sampler(device)
        , _set(set) {
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = _image.layout();
    imageInfo.imageView = _view;
    imageInfo.sampler = _sampler;

    VkWriteDescriptorSet descriptorWrites{};
    descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites.dstSet = set;
    descriptorWrites.dstBinding = 0;
    descriptorWrites.dstArrayElement = 0;
    descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites.descriptorCount = 1;
    descriptorWrites.pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(device, 1, &descriptorWrites, 0, nullptr);
}