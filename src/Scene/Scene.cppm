export module Scene;

export import Scene.Camera;
export import Scene.Object;
export import Scene.Entity;

import Geometry.Quad;

import <iostream>;
import <vector>;
import <memory>;

export struct Scene {
// private:
    CameraPtr                       _camera;
    // std::vector<ObjectPtr>          _objects;
    ObjectPtr              _object;
};

export struct DefaultScene: public Scene {
    DefaultScene() {
        std::cout << "Default scene assigned" << std::endl;
        // _objects = {ObjectPtr(new Object), ObjectPtr(new Object), ObjectPtr(new Object)};
        // _objects[0]->_geometry = GeomRef(new Quad());
        // _objects[1]->_geometry = GeomRef(new Quad());
        // _objects[2]->_geometry = GeomRef(new Quad());
        _object = ObjectPtr(new Object);
        _object->_geometry = GeomRef(new Quad());

        _camera = CameraPtr(new PerspectiveCamera(3.1415f / 3.0f, 1.0f, 0.1f, 100.0f));
    };
};

export using ScenePtr = std::shared_ptr<Scene>;