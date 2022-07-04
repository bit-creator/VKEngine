export module Scene.Entity;

import Math.Matrix4f;
import Math.Vector4f;

export class Entity {
protected:
    mth::Matrix4f                                               _modelMat;
    mth::Matrix4f												_worldMat;
    mth::Quaternion                                             _rotate;
    mth::Vector4f                                               _position;
    mth::Vector4f                                               _scale;
    bool                                                        _dirtyTransform;
    bool 		    											_dirtyWorldTransform;

public:
    void rotate(mth::Quaternion quat);
    void scale(mth::Vector4f scale);
    void translate(mth::Vector4f pos);

    mth::Matrix4f transformation();
};

void Entity::rotate(mth::Quaternion quat) {
    _rotate = quat;
    _dirtyTransform = true;
}

void Entity::scale(mth::Vector4f scale) {
    _scale = scale;
    _dirtyTransform = true;
}

void Entity::translate(mth::Vector4f pos) {
    _position = pos;
    _dirtyTransform = true;
}

mth::Matrix4f transformation() {
    if(_dirtyTransform) _modelMat.makeModel(_scale, _rotate, _position);
    return _modelMat;
}

