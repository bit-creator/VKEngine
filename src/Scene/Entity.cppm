export module Scene.Entity;

import Math.Matrix4f;
import Math.Vector3f;
import Math.Vector2f;
import Math.Quaternion4f;

namespace mth = mathon;

export class Entity {
protected:
    mth::Matrix4f                                               _modelMat;
    mth::Matrix4f												_worldMat;
    mth::Quaternion4f                                           _rotate;
    mth::Vector3f                                               _position;
    mth::Vector3f                                               _scale;
    bool                                                        _dirtyTransform;
    bool 		    											_dirtyWorldTransform;
public:
    Entity();
    Entity(const Entity& oth);
    Entity(Entity&&);
    

public:
    void rotate(mth::Quaternion4f quat);
    void scale(mth::Vector3f scale);
    void translate(mth::Vector3f pos);

    mth::Matrix4f transformation();
};


Entity::Entity():
    _rotate(0, {1, 0, 0}),  
    _position(0, 0, 0),
    _scale(1, 1, 1) {
}

Entity::Entity(const Entity& oth) {
}

Entity::Entity(Entity&& oth) {
}

void Entity::rotate(mth::Quaternion4f quat) {
    _rotate = quat;
    _dirtyTransform = true;
}

void Entity::scale(mth::Vector3f scale) {
    _scale = scale;
    _dirtyTransform = true;
}

void Entity::translate(mth::Vector3f pos) {
    _position = pos;
    _dirtyTransform = true;
}

mth::Matrix4f Entity::transformation() {
    if(_dirtyTransform) _modelMat.make_model(_scale, _rotate, _position);
    return _modelMat;
}

