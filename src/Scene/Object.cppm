export module Scene.Object;

import Scene.Entity;
import Geometry;
import <memory>;
import Math.Matrix4f;
import Math.Vector3f;
import Math.Quaternion4f;

export struct Object: public Entity {
    GeomRef                             _geometry;

    Object(GeomRef geom): _geometry(geom){}
    Object() {};
};

export using ObjectPtr = std::shared_ptr<Object>;