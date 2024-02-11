#include "Widget.hpp"
#include "Layout/Layout.hpp"
#include "../../../Utils/Physic/RayCaster.hpp"

Widget::Widget(Style::Tag tag, int evt):
	_tag(tag),
	_evt_accepted(evt)
{
	// All screen by default
	_surface = std::make_unique<Quad>();
}

void Widget::_setParent(Widget* child, Layout* parent) {
	if (!child)
		return;

	child->_parent = parent;
}

glm::vec2 Widget::getTL() const {
	return glm::vec2(x(), y());
}

glm::vec2 Widget::getBR() const {
	return glm::vec2(x() + w(), y() + h());
}

// Helpers
bool Widget::isMouseOver() const {
	return _mouse_over;
}

bool Widget::isMouseClicked() const
{
	return _mouse_clicked;
}

const Layout* Widget::parent() const {
	return _parent;
}

Style::Tag Widget::tag() const {
	return _tag;
}

bool Widget::IsIn(const Widget& widget, int x, int y) {
	if (!widget.parent())
		return false;

	const int W0 = widget.parent()->OriginalWidth();
	const int H0 = widget.parent()->OriginalHeight();

	if (W0 <= 0 || H0 <= 0)
		return false;

	// Get relative mouse pose
	glm::vec2 mouse_rel = {
		float(x) / W0,
		float(y) / H0
	};

	return RayCaster::PointInRect(mouse_rel, *widget._surface);
}

void Widget::_applyStyle() {
	if(_style.hint_x.hasValue()) x() = _style.hint_x();
	if(_style.hint_y.hasValue()) y() = _style.hint_y();
	if(_style.hint_w.hasValue()) w() = _style.hint_w();
	if(_style.hint_h.hasValue()) h() = _style.hint_h();

	_surface->material.diffuse_color = _style.background();
}

// Events propagated
void Widget::_on_mouse_button(const CommonEvents::MouseButton& evt) {
	if (_mouse_over && evt.button == MouseButton::Left && evt.action == InputAction::Pressed)
	{
		_mouse_clicked = true;
	}

	if (evt.button == MouseButton::Left && evt.action == InputAction::Released)
	{
		_mouse_clicked = false;
	}

	if (_mouse_over && evt.button == MouseButton::Left && evt.action == InputAction::Released) {
		Event::Emit(WidgetEvents::MouseClick(), this);
	}
}

void Widget::_on_mouse_move(const CommonEvents::MouseMoved& evt) {
	bool previous_mouse_over = _mouse_over;
	_mouse_over = IsIn(*this, evt.x, evt.y);

	if (_mouse_over && !previous_mouse_over) {
		Event::Emit(WidgetEvents::MouseOver(), this);
	}

	if (!_mouse_over && previous_mouse_over) {
		Event::Emit(WidgetEvents::MouseOut(), this);
	}
}

//
Style& Widget::style() {
	return _style;
}

const Style& Widget::style() const {
	return _style;
}

// Surface
Quad& Widget::surface() {
	return *_surface.get();
}

// Just to be shorter
float& Widget::x() { return _surface->x(); }
float& Widget::y() { return _surface->y(); }
float& Widget::w() { return _surface->w(); }
float& Widget::h() { return _surface->h(); }

float Widget::x() const { return _surface->x(); }
float Widget::y() const { return _surface->y(); }
float Widget::w() const { return _surface->w(); }
float Widget::h() const { return _surface->h(); }