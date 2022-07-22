export module App.Texture2D;

import Vulkan;

import Vk.Image;
import Vk.ImageView;
import Vk.Image.Sampler;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;
import Vk.CommandBuffer;

import <filesystem>;
import <memory>;

namespace fs = std::filesystem;

export enum class Uniform {
    Albedo =0,
    Normal =1
};

export class Texture2D {
    Image               _image;
    ImageView           _view;

public:
    VkDescriptorSet     _set;
    Texture2D(fs::path path, LogicalDevice device, PhysicalDevice phys, CommandBuffer buff);
    void setUniform(LogicalDevice ld, Uniform bindind, Sampler sampler, VkDescriptorSet set);
};

Texture2D::Texture2D(fs::path path, LogicalDevice device, PhysicalDevice phys, CommandBuffer buff)
        : _image(path, device, phys, buff)
        , _view(_image, _image.format(), device) {
}

void Texture2D::setUniform(LogicalDevice ld, Uniform bindind, Sampler sampler, VkDescriptorSet set) {
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = _image.layout();
    imageInfo.imageView = _view;
    imageInfo.sampler = sampler;

    VkWriteDescriptorSet descriptorWrites{};
    descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites.dstSet = set;
    descriptorWrites.dstBinding = (uint32_t)bindind;
    descriptorWrites.dstArrayElement = 0;
    descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites.descriptorCount = 1;
    descriptorWrites.pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(ld, 1, &descriptorWrites, 0, nullptr);
}

export using TextureRef = std::shared_ptr<Texture2D>;