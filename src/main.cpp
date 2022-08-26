/**
 * @file main.cpp
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  main file, the programm starts here
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// export module Main;
import App.RenderDevice;
import Scene;
import Geometry;
import Geometry.Quad;
import Scene.Entity;
import Scene.Object;
import App.Event;

import <memory>;
import <iostream>;
import <cmath>;
import <chrono>;
import <thread>;

struct SceneUpdater final: public Event {
    ScenePtr            scene;
    double              time =0.0;

    void onRender() override {
        // std::this_thread::sleep_for(std::chrono::milliseconds(4));
        time += 0.001;
        float x = std::abs(std::sin(time));
        float y = std::abs(std::cos(time));
        // scene->_object->scale({1, 1, 1});
    }
};

int main() {
    RenderDevice& device = RenderDevice::device();

    auto upd = new SceneUpdater;

    ScenePtr scene = ScenePtr(new DefaultScene); 
    upd->scene = scene;
    device.addEvent(EventPtr(upd));

    device.execute(scene);

    return 0;
}