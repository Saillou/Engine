#include "BaseWidget.hpp"

BaseWidget::BaseWidget() {
	_subscribe(&BaseWidget::_on_mouse_button);
}

glm::vec4 BaseWidget::Transparent() {
    return glm::vec4(0, 0, 0, 0);
}

void BaseWidget::_onClick() {
	// ..
}

void BaseWidget::_on_mouse_button(const CommonEvents::MouseButton& evt) {
	if (evt.button == Button::Left && evt.action == Action::Pressed) {
		// TODO: check if mouse is in the shape
		_emit(evt);
	}
}