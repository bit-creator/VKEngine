export module App.Event;

import <memory>;

export struct Event {
    virtual void onRender() {  }
    virtual void onMouseMove([[maybe_unused]]double x, [[maybe_unused]]double y) {  }
    
    virtual void onMouseClick([[maybe_unused]]int button,
                            [[maybe_unused]]int action,
                            [[maybe_unused]]int mode) {  }
    
    virtual void onKeyPressed([[maybe_unused]]int key, 
                                [[maybe_unused]]int scancode,
                                [[maybe_unused]]int action,
                                [[maybe_unused]]int mode) {  }
};

export using EventPtr = std::shared_ptr<Event>;