#include "Widget.hpp"
#include "Layout/Layout.hpp"
#include "../../../Utils/RayCaster.hpp"

Widget::Widget(Style::Tag tag, int evt):
	_tag(tag)
{
	if(evt & EventListened::MouseButton)
		_subscribe(&Widget::_on_mouse_button);

	if (evt & EventListened::MouseMove)
		_subscribe(&Widget::_on_mouse_move);

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
	if(_style.has_hint_x()) x() = _style.hint_x();
	if(_style.has_hint_y()) y() = _style.hint_y();
	if(_style.has_hint_w()) w() = _style.hint_w();
	if(_style.has_hint_h()) h() = _style.hint_h();

	_surface->material.diffuse_color = _style.background();
}

// Callbacks
void Widget::_on_mouse_button(const CommonEvents::MouseButton& evt) {
	if (evt.button == MouseButton::Left && evt.action == InputAction::Released) {
		if (IsIn(*this, evt.x, evt.y)) {
			Event::Emit(evt, this);
		}
	}
}

void Widget::_on_mouse_move(const CommonEvents::MouseMoved& evt) {
	_mouse_over = IsIn(*this, evt.x, evt.y);
}

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