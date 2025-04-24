#pragma once

#include "PanelFight.hpp"
#include <algorithm>

PanelFight::PanelFight(const Scene& scene) : 
    Panel(),
    _hit_tween(Animator::Tweet(0.0f, 0.3f, Animator::Tweet::Type::Quadratic))
{
    reset();
}
void PanelFight::reset()
{
    _current_mouse_x = 0;
    _current_mouse_y = 0;
    
    _current_pos_x = 0;
    _current_pos_y = 0;
    
    _current_speed_x = 0;
    _current_speed_y = 0;

    _current_damage = 0;
    _n_enemies = 0;
    _expe_gain = 0;

    _particle_timer.tic();
    _enemies_timer.tic();
    _hit_tween.reset();

    _particles.clear();
    _enemies.clear();
}
void PanelFight::draw(CanvasEntity& canvas, const Scene& scene)
{
    const ModelFight& data = shared_data().fight;
    const float WIDTH = (float)scene.width();
    const float HEIGHT = (float)scene.height();

    _generate_ennemies();
    if (_enemies.empty() && _n_enemies == data.enemies_total) {
        Event::Emit(FightEvents::Result(FightEvents::Result::Win), this);
        return reset();
    }    
    _evolve_particles();
    _compute_current_pos();
    _compute_collisions();
    if (_current_damage >= data.total_life) {
        Event::Emit(FightEvents::Result(FightEvents::Result::Lose), this);
        return reset();
    }

    float k = _hit_tween.update(0.f, 1.f);

    const auto size = data.size;
    canvas.get()
        .begin()
        .rect(_current_pos_x - size/2.0f, HEIGHT - _current_pos_y - size/2.0f, size, size)
        .fill(CanvasShape::Color(42, 42, 42, 255))
        .stroke(CanvasShape::Color(255, 255*k, 255*k, 127), 2.0f);

    float r = std::min(_particle_timer.elapsed<Timer::millisecond>() - data.time_reload, 0.0f) / data.time_reload;
    float l = std::max(data.total_life - _current_damage, 0.0f) / data.total_life;
    float a = 1.0f - std::exp(-_expe_gain / ((data.time_reload / data.enemies_reload) * (data.enemies_total / data.exp_enemies)));

    canvas.get()
        .begin()
        .rect(_current_pos_x - size / 2.0f, HEIGHT - _current_pos_y - size / 2.0f - 15.0f, size + size * r, 10.0f)
        .fill(CanvasShape::Color(142, 42, 42, 255))
        .stroke(CanvasShape::Color(255, 255, 255, 127), 2.0f)
        .rect(_current_pos_x - size / 2.0f, HEIGHT - _current_pos_y - size / 2.0f - 15.0f, size, 10.0f)
        .stroke(CanvasShape::Color(255, 255, 255, 127), 2.0f);

    canvas.get()
        .begin()
        .rect(_current_pos_x - size / 2.0f, HEIGHT - _current_pos_y - size/2.0f  - 25.0f, size * l, 10.0f)
        .fill(CanvasShape::Color(42, 142, 42, 255))
        .stroke(CanvasShape::Color(255, 255, 255, 127), 2.0f)
        .rect(_current_pos_x - size / 2.0f, HEIGHT - _current_pos_y - size / 2.0f - 25.0f, size, 10.0f)
        .stroke(CanvasShape::Color(255, 255, 255, 127), 2.0f);

    canvas.get()
        .begin()
        .rect(_current_pos_x - size / 2.0f, HEIGHT - _current_pos_y - size / 2.0f - 35.0f, size * a, 10.0f)
        .fill(CanvasShape::Color(142, 42, 142, 255))
        .stroke(CanvasShape::Color(255, 255, 255, 127), 2.0f)
        .rect(_current_pos_x - size / 2.0f, HEIGHT - _current_pos_y - size / 2.0f - 35.0f, size, 10.0f)
        .stroke(CanvasShape::Color(255, 255, 255, 127), 2.0f);

    for (const Particle& p : _particles) {
        canvas.get()
            .begin()
            .circle(p.x, HEIGHT - p.y, size/10.0f)
            .fill(p.color)
            .stroke(CanvasShape::Color(255, 255, 255, 127), 2.0f);
    }

    for (const Particle& p : _enemies) {
        canvas.get()
            .begin()
            .circle(p.x, HEIGHT - p.y, data.size_enemies/10.f)
            .fill(p.color)
            .stroke(CanvasShape::Color(255, 255, 255, 127), 2.0f);
    }

    canvas.get()
        .begin()
        .rect(WIDTH - 250.0f - 50.0f, HEIGHT - 50.0f, 250.0f * (1.0f - _n_enemies / (float)data.enemies_total), 20.0f)
        .fill(CanvasShape::Color(150, 150, 160, 255))
        .rect(WIDTH - 250.0f - 50.0f, HEIGHT - 50.0f, 250.0f, 20.0f)
        .stroke(CanvasShape::Color(255, 255, 255, 127), 2.0f);
}
void PanelFight::mouse_clicked(const CommonEvents::MouseButton& btn) 
{
    _generateParticle(_current_pos_x, _current_pos_y);
}
void PanelFight::mouse_moved(const CommonEvents::MouseMoved& evt) {
    _current_mouse_x = evt.x;
    _current_mouse_y = evt.y;
}

void PanelFight::_compute_current_pos() {
    const ModelFight& data = shared_data().fight;

    const float dx = (_current_mouse_x - _current_pos_x);
    const float dy = (_current_mouse_y - _current_pos_y);
    const float n = sqrt(dx * dx + dy * dy);

    constexpr auto sign = [](float v) {
        return v > 0 ? 1 : v < 0 ? -1 : 0;
    };

    float adx = std::abs(dx);
    float ady = std::abs(dy);

    if (n > 0) {
        _current_speed_x += data.acc * adx / n;
        _current_speed_y += data.acc * ady / n;

        if (adx > _current_speed_x)
            _current_pos_x += sign(dx) * _current_speed_x;
        else {
            _current_pos_x = _current_mouse_x;
            _current_speed_x = 0;
        }

        if (ady > _current_speed_y)
            _current_pos_y += sign(dy) * _current_speed_y;
        else {
            _current_pos_y = _current_mouse_y;
            _current_speed_y = 0;
        }
    }
}

void PanelFight::_compute_collisions() {
    const ModelFight& data = shared_data().fight;

    float hs = data.size / 2 + 4;

    for (Particle& p : _enemies) {
        // enemies killed
        for (Particle& pp : _particles) {
            float r = sqrt((p.x - pp.x) * (p.x - pp.x) + (p.y - pp.y) * (p.y - pp.y));
            if (r < (data.size_enemies + data.size)/10.0f)
            {
                _expe_gain += data.exp_enemies;
                pp.x = 2000; // flagged as to be removed
                p.x = -500; // flagged as to be removed
            }
        }

        // touched
        if((p.x > _current_pos_x - hs && p.x < _current_pos_x + hs) && (p.y > _current_pos_y - hs && p.y < _current_pos_y + hs)) 
        {
            _current_damage += data.enemies_damage;
            p.x = -500; // flagged as to be removed
            _hit_tween.reset();
        }
    }

    _particles.erase(std::remove_if(_particles.begin(), _particles.end(), [](const Particle& p) -> bool {
        return p.x > 1600;
    }), _particles.end());

    _enemies.erase(std::remove_if(_enemies.begin(), _enemies.end(), [](const Particle& p) -> bool {
        return p.x < 0;
    }), _enemies.end());
}

void PanelFight::_generateParticle(int x, int y) 
{
    const ModelFight& data = shared_data().fight;

    if (_particle_timer.elapsed<Timer::millisecond>() > (int)data.time_reload - shared_data().idle.count) {
        _particle_timer.tic();
        _particles.push_back({ (float)x + data.size/2.0f, (float)y, CanvasShape::Color(42, 42, 50, 255) });
    }
}

void PanelFight::_evolve_particles()
{
    const ModelFight& data = shared_data().fight;

    for (Particle& p : _particles) {
        p.x += data.part_speed;
    }

    for (Particle& p : _enemies) {
        p.x -= 1.0f;
    }
}

void PanelFight::_generate_ennemies()
{
    const ModelFight& data = shared_data().fight;

    if (_enemies_timer.elapsed<Timer::millisecond>() > data.enemies_reload && _n_enemies < data.enemies_total) {
        _enemies_timer.tic();
        _enemies.push_back({ 
            1600.0f, 
            500.0f + 250.0f*std::sin(6.28f * (Timer::GetCurrentTime<Timer::millisecond>() % 300) / 300.f),
            CanvasShape::Color(142, 142, 182, 255)
        });
        _n_enemies++;
    }
}
