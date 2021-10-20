/**
 * @file ColorAttachment.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on color attachments
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.ColorAttachment;

import Vulkan;
import Vk.Swapchain;

export class ColorAttachment {
private:
    VkFormat                            _format;
    VkSampleCountFlagBits               _samples;

public:
    ColorAttachment(const Swapchain& swapchain);
    ~ColorAttachment();

    VkAttachmentDescription
    getDescription();

    VkAttachmentReference
    getRef();
};


ColorAttachment::ColorAttachment(const Swapchain& swapchain)
    : _format(swapchain.getFormat().format)
    , _samples(VK_SAMPLE_COUNT_1_BIT)
{

}

ColorAttachment::~ColorAttachment() {

}

VkAttachmentDescription
ColorAttachment::getDescription() {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = _format;
    colorAttachment.samples = _samples;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    return colorAttachment;
}

// VkAttachmentReference
// ColorAttachment::getReference() {
//     VkAttachmentReference colorAttachmentRef{};
//     colorAttachmentRef.attachment = 0;
//     colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

// }