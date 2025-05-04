#pragma once

#include <Engine/Framework/Service.hpp>
#include <Engine/Events/Events.hpp>
#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Window.hpp>
#include <Engine/Graphic/Base/Widget/Text.hpp>
#include <Engine/Utils/Animator.hpp>

#include <unordered_map>
#include "CanvasEntity.hpp"

struct Sprite : Event::Subscriber {
    Sprite();
    virtual ~Sprite();

    virtual void draw();
    virtual void clear();

    std::shared_ptr<Sprite> get(const std::string& name) const;
    std::shared_ptr<Sprite> add(const std::string& name, std::shared_ptr<Sprite> sprite = nullptr);
    void remove(const std::string& name);
    void remove_all();

    float width() const;
    float height() const;

protected:
    virtual void onMouseOver()      {};
    virtual void onMouseOut()       {};
    virtual void onMouseReleased()  {};
    virtual void onMousePressed()   {};
    virtual void onMouseMoved(int x, int y) {};

    virtual bool _hitArea(int x, int y);

    const Scene& _scene;
    CanvasEntity _canvas;

    std::unordered_map<std::string, std::shared_ptr<Sprite>> _children;

    bool _is_mouse_over() const {
        return __is_mouse_over;
    }

private:
    bool __is_mouse_over = false;
};