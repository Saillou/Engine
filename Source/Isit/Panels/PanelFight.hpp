#pragma once

#include "Panel.hpp"

struct FightEvents : public Event
{
    struct Result : public _Base
    {
        enum _list_results {
            Unkwnown,
            Lose, 
            Win,
        } _result;

        Result(_list_results r = Unkwnown) : 
            _Base(_Type::EventFightResult), _result(r)
        { }
    };

private:
    enum _Type : int {
        EventFightResult,
    };
};

struct PanelFight : public Panel 
{
    explicit PanelFight(const Scene& scene);

    void reset() override;

    void draw(CanvasEntity& canvas, const Scene& scene) override;
    void mouse_clicked(const CommonEvents::MouseButton& btn) override;
    void mouse_moved(const CommonEvents::MouseMoved& evt) override;

private:
    void _compute_current_pos();
    void _compute_collisions();
    void _generateParticle(int x, int y);
    void _evolve_particles();
    void _generate_ennemies();

    int _current_mouse_x = 0;
    int _current_mouse_y = 0;

    float _current_pos_x = 0;
    float _current_pos_y = 0;

    float _current_speed_x = 0;
    float _current_speed_y = 0;

    float _current_damage = 0;
    size_t _n_enemies = 0;
    float _expe_gain = 0;

    struct Particle {
        float x;
        float y;
        CanvasShape::Color color;
    };
    std::vector<Particle> _particles;
    std::vector<Particle> _enemies;
    Timer::Chronometre _particle_timer;
    Timer::Chronometre _enemies_timer;
    Animator::Tweet _hit_tween;
};