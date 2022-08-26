export module App.Resource;

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

enum class Uniform;

export class Resource {
protected:
    virtual      ~Resource() {  };
};

export using ResourceRef = std::shared_ptr<Resource>;

export struct RTexture2D: public Resource {
    Image               _image;
    ImageView           _view;

public:
    RTexture2D( fs::path path, LogicalDevice device,
                PhysicalDevice phys, CommandBuffer buff):
        _image(path, device, phys, buff),
        _view(_image, _image.format(), device) {  };
    
    virtual      ~RTexture2D() {  };
};

export using RTexture2DRef = std::shared_ptr<RTexture2D>;

// RTexture2D::RTexture2D(fs::path path, LogicalDevice device, PhysicalDevice phys, CommandBuffer buff)
//         : _image(path, device, phys, buff)
//         , _view(_image, _image.format(), device) {
// }