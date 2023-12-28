#include "BaseBody.hpp"

BaseBody::BaseBody(ContactType type_, glm::vec3 position_, glm::vec3 dimensions_, glm::vec3 orientation_):
    type(type_),
    position(position_),
    dimensions(dimensions_),
    orientation(orientation_)
{
    // ..
}

BaseBody::~BaseBody() {
    // ..
}