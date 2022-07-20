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
    Sampler             _sampler;

public:
    VkDescriptorSet     _set;
    Texture2D(fs::path path, LogicalDevice device, PhysicalDevice phys, CommandBuffer buff, VkDescriptorSet set);
    void setUniform(Uniform bindind, Sampler sampler, VkDescriptorSet set);
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

void Texture2D::setUniform(Uniform bindind, Sampler sampler, VkDescriptorSet set) {
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = _image.layout();
    imageInfo.imageView = _view;
    imageInfo.sampler = _sampler;

    VkWriteDescriptorSet descriptorWrites{};
    descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites.dstSet = set;
    descriptorWrites.dstBinding = (uint32_t)bindind;
    descriptorWrites.dstArrayElement = 0;
    descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites.descriptorCount = 1;
    descriptorWrites.pImageInfo = &imageInfo;

}

export using TextureRef = std::shared_ptr<Texture2D>;