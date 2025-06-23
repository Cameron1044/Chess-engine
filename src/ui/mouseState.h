#pragma once

#include <cstdlib>
#include <iostream>

class MouseState {
    public:
        // Setters
        void setPressed(int x, int y);
        void setNotPressed(int x, int y);
        void setWhilePressed(int x, int y);

        // Const Getters
        bool isPressed() const;
        int getxPressed(bool file) const;
        int getyPressed(bool rank) const;
        int getxReleased(bool file) const;
        int getyReleased(bool rank) const;
        int getxWhilePressed() const;
        int getyWhilePressed() const;

    private:
        int xPressed_m, yPressed_m;
        int xReleased_m, yReleased_m;
        int xWhilePressed_m, yWhilePressed_m;
        bool isPressed_m;
};