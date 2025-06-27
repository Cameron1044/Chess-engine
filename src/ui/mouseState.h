#pragma once

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
        int xPressed_m = 0;
        int yPressed_m = 0;
        int xReleased_m = 0;
        int yReleased_m = 0;
        int xWhilePressed_m = 0;
        int yWhilePressed_m = 0;
        bool isPressed_m = false;
};