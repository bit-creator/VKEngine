export module Scene.Camera.Control;

import GLFW;
import Scene.Camera;
import Math.Vector3f;
import Math.Vector4f;
import Math.Matrix4f;
import Math.Quaternion4f;
import App.Event;
import <memory>;
import <cmath>;
import <iostream>;

namespace mth = mathon;

export class OrbitalCtrl: public Event {
private:
    CameraPtr                       _camera;
    mth::Vector3f                   _direction;
    mth::Vector3f                   _angles;
    float                           _velocity;

public:
	OrbitalCtrl(CameraPtr camera) noexcept
		: _camera(camera)
		, _direction(0.0f, 0.0f, 0.0f)
		, _angles(0.0f, 0.0f, 0.0f)
		, _velocity(0.05f)
	{  }

    virtual ~OrbitalCtrl() noexcept {
	}

    void setVelocity(float velocity) noexcept {
		_velocity = velocity;
	}

public:
	void onRender() noexcept override {
		auto posDelta = _velocity * (_camera->transformation() * mth::Vector4f(_direction)).xyz();
	    auto position = _camera->position() + posDelta;

		_camera ->rotate(mth::Quaternion4f(_angles));
	    _camera ->translate(position);
	}
    
	void onKeyPressed(int key, int scancode, int action, int mode) noexcept override {
		if (action == GLFW_PRESS) {
			if (key == GLFW_KEY_W) _direction.z(-1);
			if (key == GLFW_KEY_S) _direction.z(1);
			if (key == GLFW_KEY_A) _direction.x(-1);
			if (key == GLFW_KEY_D) _direction.x(1);
		}

		if (action == GLFW_RELEASE) {
			if (key == GLFW_KEY_D || key == GLFW_KEY_A) _direction.x(0);
			if (key == GLFW_KEY_W || key == GLFW_KEY_S) _direction.z(0);
		}
	}
    
	void onMouseMove(double x, double y) noexcept override {
		_angles += mth::Vector3f(-y * 0.002, x * 0.002, 0.);
		if(_angles.x() > M_PI / 2)    _angles.x(M_PI / 2);
		if(_angles.x() < -M_PI / 2)   _angles.x(-M_PI / 2);
	}
};

// OrbitalCtrl::OrbitalCtrl(CameraPtr camera) noexcept
// 	: _camera(camera)
// 	, _direction(0.0f, 0.0f, 0.0f)
// 	, _angles(0.0f, 0.0f, 0.0f)
// 	, _velocity(0.05f)
// {  }

// OrbitalCtrl::~OrbitalCtrl() noexcept {
// }

// void OrbitalCtrl::onRender() noexcept {
//     auto posDelta = _velocity * (_camera->transformation() * mth::Vector4f(_direction)).xyz();
//     auto position = _camera->position() + posDelta;

// 	_camera ->rotate(mth::Quaternion4f(_angles));
//     _camera ->translate(position);
// }

// void OrbitalCtrl::onKeyPressed(int 			key, int scancode, int action, int mode) noexcept {
// 	if (action == GLFW_PRESS)
// 	{
// 		if (key == GLFW_KEY_W) _direction.z(1);
// 		if (key == GLFW_KEY_S) _direction.z(-1);
// 		if (key == GLFW_KEY_A) _direction.x(-1);
// 		if (key == GLFW_KEY_D) _direction.x(1);
// 	}

// 	if (action == GLFW_RELEASE)
// 	{
// 		if (key == GLFW_KEY_D || key == GLFW_KEY_A) _direction.x(0);
// 		if (key == GLFW_KEY_W || key == GLFW_KEY_S) _direction.z(0);
// 	}
// }

// void OrbitalCtrl::onMouseMove (double x, double y) noexcept {
// 	_angles += mth::Vector3f(y * 0.002, x * 0.002, 0.);
// 	if(_angles.x() > M_PI / 2)    _angles.x(M_PI / 2);
// 	if(_angles.x() < -M_PI / 2)   _angles.x(-M_PI / 2);
// }

// void OrbitalCtrl::setVelocity(float velocity) noexcept {
// 	_velocity = velocity;
// }
