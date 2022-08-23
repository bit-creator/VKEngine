export module Scene;

export import Scene.Camera;
export import Scene.Object;
export import Scene.Entity;
import Material;
import App.Texture2D;

import Geometry.Quad;

import <iostream>;
import <filesystem>;
import <vector>;
import <memory>;

namespace fs = std::filesystem;

export struct Scene {
// private:
    CameraPtr                       _camera;
    std::vector<ObjectPtr>          _objects;
    // ObjectPtr                       _object;
};

export struct DefaultScene: public Scene {
    DefaultScene() {
        std::cout << "Default scene assigned" << std::endl;
        _objects = {ObjectPtr(new Object), ObjectPtr(new Object), ObjectPtr(new Object)};
        auto quad = GeomRef(new Quad());
        _objects[0]->_geometry = quad;
        _objects[1]->_geometry = quad;
        _objects[2]->_geometry = quad;
        auto material = MaterialPtr (
            new Material {
                Texture2D(fs::path("/home/user/Code/C++/VKEngine/assets/textures/brick+wall-512x512.jpg"))
            }
        );

        _objects[0]->_material = material;
        _objects[1]->_material = material;
        _objects[2]->_material = material;

        _objects[0]->scale({2, 1, 1});
        _objects[0]->translate({0, 0, 0.5});

        _objects[1]->scale({1, 2, 1});
        _objects[1]->translate({0, 0, 0.7});

        _objects[2]->scale({0.5, 0.5, 1});
        _objects[2]->translate({0, 0, 0.3});

        _camera = CameraPtr(new PerspectiveCamera(3.1415f / 3.0f, 1.0f, 0.1f, 100.0f));
    };
};

export using ScenePtr = std::shared_ptr<Scene>;