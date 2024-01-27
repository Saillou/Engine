#include "Style.hpp"

const float Style::Unset() 
{
    return -1.0f;
}

const glm::vec4 Style::Transparent()
{
    return glm::vec4(0);
}

Style Style::operator+(const Style& styleNew) 
{
    return styleNew;
}
