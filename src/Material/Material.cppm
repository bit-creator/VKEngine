export module Material;

import <memory>;
import App.Texture2D;

export struct Material {
    TextureRef              _albedo;
};

export using MaterialPtr = std::shared_ptr<Material>;
