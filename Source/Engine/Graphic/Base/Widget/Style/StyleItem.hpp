#pragma once

struct Style;

template<class T>
class StyleItem 
{
public:
    StyleItem(Style& style, T defaultValue)
        : m_defaultValue(defaultValue)
        , m_hasValue(false)
        , m_style(style)
    {
    }

    Style& setValue(T v)
    {
        m_value = v;
        m_hasValue = true;
        return m_style;
    }

    void clearValue()
    {
        m_hasValue = false;
    }

    bool hasValue() const { return m_hasValue; }

    const T& StyleItem::operator()() const
    {
        return m_hasValue ? m_value : m_defaultValue;
    }

    void StyleItem::operator=(const StyleItem& other)
    {
        m_value = other.m_value;
        m_defaultValue = other.m_defaultValue;
        m_hasValue = other.m_hasValue;
    }

    void StyleItem::operator+(const StyleItem<T>& other)
    {
        m_value = m_hasValue ? m_value : other();
        if (!m_hasValue)
            m_hasValue = other.hasValue();
    }

    void StyleItem::operator+=(const StyleItem<T>& other)
    {
        m_value = m_hasValue ? m_value : other();
        if (!m_hasValue)
            m_hasValue = other.hasValue();
    }

private:
    T m_value;
    T m_defaultValue;
    bool m_hasValue;
    Style& m_style;
};