#include "BaseWidget.hpp"
#include "BaseLayout.hpp"
#include "../../../Utils/RayCaster.hpp"

BaseWidget::BaseWidget() {
	_subscribe(&BaseWidget::_on_mouse_button);
}

// Helpers
glm::vec4 BaseWidget::Transparent() {
    return glm::vec4(0, 0, 0, 0);
}

const BaseLayout* BaseWidget::parent() const
{
	return _parent;
}

bool BaseWidget::IsIn(const BaseWidget& widget, int x, int y) {
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
void BaseWidget::_on_mouse_button(const CommonEvents::MouseButton& evt) {
	if (evt.button == Button::Left && evt.action == Action::Pressed) {
		if (IsIn(*this, evt.x, evt.y)) {
			_emit(evt);
		}
	}
}