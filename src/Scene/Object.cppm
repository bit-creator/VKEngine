export module Scene.Object;

import Scene.Entity;
import Geometry;
import <memory>;

export struct Object: public Entity {
    GeomRef                             _geometry;
};