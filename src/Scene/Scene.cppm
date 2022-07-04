export module Scene;

import module Scene.Camera;

export class Scene {
private:
    Camera              _camera = PerspectiveCamera(3.1415f / 3.0f, 1.0f, 0.1f, 100.0f);
    
};