#include "StyleSheet.hpp"

StyleSheet StyleSheet::CreateDefault()
{
	StyleSheet ss;

	return ss;
}

std::optional<Style> StyleSheet::getStyle(const Widget* pWidget)
{
	if (!pWidget)
		return {};

	const auto& tag		  = pWidget->_tag;
	const auto& className = pWidget->className;

	Style styleTag;
	if (m_tag_styles.find((int)pWidget->_tag) != m_tag_styles.cend()) {
		styleTag = m_tag_styles[(int)pWidget->_tag];
	}

	Style styleClass;
	if (m_class_styles.find(pWidget->className) != m_class_styles.cend()) {
		styleClass = m_class_styles[pWidget->className];
	}

	Style styleId;
	if (m_class_styles.find(pWidget->idName) != m_class_styles.cend()) {
		styleId = m_class_styles[pWidget->idName];
	}
	
	// Priorities: User defined (Widget::style()), then id, then class, then tag.
	return (((styleTag + styleClass) + styleId) + pWidget->style());
}

void StyleSheet::applyStyle(Widget* widget) {
	if (!widget)
		return;

	auto widget_style = getStyle(widget);

	if (widget_style.has_value())
		widget->style() = widget_style.value();
}

StyleSheet& StyleSheet::addRule(Style::Tag tag, const Style& style) {
	m_tag_styles[(int)tag] = style;
	return *this;
}

StyleSheet& StyleSheet::addRule(const std::string& className, const Style& style) {
	m_class_styles[className] = style;
	return *this;
}
