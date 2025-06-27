#include "ui/mouseState.h"

void MouseState::setPressed(int x, int y) {
    isPressed_m = true;
    xPressed_m = x;
    yPressed_m = y;
    xWhilePressed_m = xPressed_m;
    yWhilePressed_m = yPressed_m;
}

void MouseState::setNotPressed(int x, int y){
    isPressed_m = false;
    xReleased_m = x;
    yReleased_m = y;
}

void MouseState::setWhilePressed(int x, int y) {
    xWhilePressed_m = x;
    yWhilePressed_m = y;
}

bool MouseState::isPressed() const {
    return isPressed_m;
}

int MouseState::getxPressed(bool file = false) const {
    if (file) {return xPressed_m/100;}
    return xPressed_m;
}

int MouseState::getyPressed(bool rank = false) const {
    if (rank) {return abs(yPressed_m/100 - 7);}
    return yPressed_m;
}

int MouseState::getxReleased(bool file = false) const {
    if (file) {return xReleased_m/100;}
    return xReleased_m;
}

int MouseState::getyReleased(bool rank = false) const {
    if (rank) {return abs(yReleased_m/100 - 7);}
    return yReleased_m;
}

int MouseState::getxWhilePressed() const {
    return xWhilePressed_m;
}

int MouseState::getyWhilePressed() const {
    return yWhilePressed_m;
}