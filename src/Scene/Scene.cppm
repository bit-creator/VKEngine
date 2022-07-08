export module Scene;

export import Scene.Camera;
export import Scene.Object;
import Math.Vector3f;
import Geometry.Quad;

import <memory>;

export struct Scene {
// private:
    using pointer = std::shared_ptr<Scene>;

    Camera              _camera;
    Object              _object;

    static pointer default_scene() {
        Object obj;
        obj._geometry = GeomRef(new Quad());
        Camera cam = PerspectiveCamera(3.1415f / 3.0f, 1.0f, 0.1f, 100.0f);
        pointer scene = pointer(new Scene{cam, obj});
        // scene._camera = PerspectiveCamera(3.1415f / 3.0f, 1.0f, 0.1f, 100.0f);
        // scene._object = {new Quad()}
        return scene;
    };
};

export using ScenePtr = Scene::pointer;