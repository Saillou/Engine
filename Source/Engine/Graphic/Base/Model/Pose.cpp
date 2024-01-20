#include "Pose.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

//glm::mat4 Pose::getMat4() const {
    //glm::mat4 model(1.0f);

    //model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
    //model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
    //model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));

    //model = glm::translate(model, position);

    //model = glm::scale(model, scale);
    //return model;
//}

//void Pose::setMat4(const glm::mat4& quat) {
    //glm::qua orientation;
    //glm::decompose(quat, scale, orientation, position, glm::vec3{}, glm::vec4{});
//}

Pose::Pose(const glm::mat4& q):
    _quat(q)
{
    // ..
}

glm::mat4& Pose::mat4()
{
    return _quat;
}

const glm::mat4& Pose::mat4() const
{
    return _quat;
}
