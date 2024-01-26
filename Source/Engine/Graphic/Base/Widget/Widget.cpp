#include "Widget.hpp"
#include "BaseLayout.hpp"
#include "../../../Utils/RayCaster.hpp"

Widget::Widget(Tag tag, int evt):
	_tag(tag)
{
	if(evt & EventListened::MouseButton)
		_subscribe(&Widget::_on_mouse_button);

	if (evt & EventListened::MouseMove)
		_subscribe(&Widget::_on_mouse_move);
}

// Helpers
glm::vec4 Widget::Transparent() {
    return glm::vec4(0, 0, 0, 0);
}

bool Widget::isMouseOver() const {
	return _mouse_over;
}

const BaseLayout* Widget::parent() const {
	return _parent;
}

bool Widget::IsIn(const Widget& widget, int x, int y) {
	if (!widget.parent())
		return false;

	if (widget.parent()->width() <= 0 || widget.parent()->height() <= 0)
		return false;

	// Get relative mouse pose
	glm::vec2 mouse_rel = {
		float(x) / widget.parent()->width(),
		float(y) / widget.parent()->height()
	};

	for (auto& surface : widget._surfaces) {
		if (RayCaster::PointInRect(mouse_rel, *surface)) {
			return true;
		}
	}

	return false;
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
