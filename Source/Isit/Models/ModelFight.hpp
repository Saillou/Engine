#pragma once

struct ModelFight {
    float size = 50.0f;
    float size_enemies = 50.0f;
    float enemies_damage = 500.0f;
    float acc = 1.0f;         // px/frame
    float part_speed = 3.f;   // px/frame
    float time_reload = 1000; // ms
    float total_life = 1000;
    size_t enemies_total = 300;
    float exp_enemies = 10.0f;
    float enemies_reload = 300;
};