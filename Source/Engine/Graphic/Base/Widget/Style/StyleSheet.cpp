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
	const auto& className = pWidget->_className;

	Style styleTag;
	if (m_tag_styles.find((int)pWidget->_tag) != m_tag_styles.cend()) {
		styleTag = m_tag_styles[(int)pWidget->_tag];
	}

	Style styleClass;
	if (m_class_styles.find(pWidget->_className) != m_class_styles.cend()) {
		styleTag = m_class_styles[pWidget->_className];
	}
	
	return styleTag + styleClass + pWidget->style();
}

void StyleSheet::applyStyle(Widget* widget) {
	if (!widget)
		return;

	auto widget_style = getStyle(widget);

	if (widget_style.has_value())
		widget->style() = widget_style.value();
}

void StyleSheet::addRule(int tag, const Style& style) {
	m_tag_styles[tag] = style;
}

void StyleSheet::addRule(const std::string& className, const Style& style) {
	m_class_styles[className] = style;
}
