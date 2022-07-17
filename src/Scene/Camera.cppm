export module Scene.Camera;

import <memory>;
import Scene.Entity;
import Math.Matrix4f;
import Math.Vector3f;
import Math.Quaternion4f;

namespace mth = mathon;

export class Camera: public Entity {
protected:
    mth::Matrix4f              projection;

public:
    Camera(mth::Matrix4f proj);

    mth::Matrix4f ViewProjection();
    mth::Matrix4f Projection();
    mth::Matrix4f View();
};

// export struct OrthoCamera: public Camera {
//     OrthoCamera(float left, float right, float bottom, float top, float near, float far);
// };

export struct PerspectiveCamera: public Camera {
    PerspectiveCamera(float fovy, float aspect, float near, float far);
};

export struct CustomCamera: public Camera {
    CustomCamera(mth::Matrix4f proj);
};

export using CameraPtr = std::shared_ptr<Camera>;

Camera::Camera(mth::Matrix4f proj)
    : projection(proj) {
}

mth::Matrix4f Camera::ViewProjection() {

}

mth::Matrix4f Camera::Projection() {

}

mth::Matrix4f Camera::View() {
    
}

// OrthoCamera::OrthoCamera(float left, float right, float bottom, float top, float near, float far)
//     : Camera(mth::Matrix4f {

//     }){
// }

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far)
    : Camera(mth::Matrix4f{}){
        projection.make_perspective(fovy, aspect, near, far);
}

CustomCamera::CustomCamera(mth::Matrix4f proj)
    : Camera(proj){
}
