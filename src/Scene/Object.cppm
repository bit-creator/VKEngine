export module Scene.Object;

import Scene.Entity;
import Geometry;
import <memory>;

export struct Object: public Entity {
    GeomRef                             _geometry;

    Object(GeomRef geom): _geometry(geom){}
    Object() {};
};

export using ObjectPtr = std::shared_ptr<Object>;