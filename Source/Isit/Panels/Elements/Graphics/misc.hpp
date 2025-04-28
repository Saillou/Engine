#pragma once

struct Point {
    float x = 0.0f;
    float y = 0.0f;
};

struct Circle {
    float x = 0.0f;
    float y = 0.0f;
    float r = 0.0f;

    bool is_in(int mx, int my) {
        float dx = mx - (x + r/2);
        float dy = my - (y + r/2);
        return (dx* dx + dy*dy) < r*r;
    }
};

struct Rectangle {
    float x = 0.0f;
    float y = 0.0f;
    float w = 0.0f;
    float h = 0.0f;

    bool is_in(int mx, int my) {
        return (mx > x && mx < x + w) && (my > y && my < y + h);
    }
};