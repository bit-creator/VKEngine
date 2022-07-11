export module App.Event;

import <memory>;

export struct Event {
    virtual void onRender() {  }
    virtual void onMouseMove(double x, double y) {  }
    virtual void onMouseClick(int button, int action, int mode) {  }
    virtual void onKeyPressed(int key, int scancode, int action, int mode) {  }
};

export using EventPtr = std::shared_ptr<Event>;