#include "RayTracer.hpp"

#include <limits>

glm::vec2 RayTracer::GetScreenPosition(const Camera& camera, const glm::vec3& worldPoint)
{
	glm::vec3 view_space = glm::vec3(camera.projection * camera.modelview * glm::vec4(worldPoint, 1.0));
	
	if (std::abs(view_space.z) <= std::numeric_limits<float>::epsilon())
		return view_space;

	float kz = camera.direction.z / view_space.z;
	glm::vec3 screen_projection = view_space - kz * camera.direction;

	printf("cam: %f %f %f \n", camera.direction.x, camera.direction.y, camera.direction.z);
	printf("view: %f %f %f \n",   view_space.x, view_space.y, view_space.z);
	printf("proj: %f %f %f \n\n", screen_projection.x, screen_projection.y, screen_projection.z);

	return screen_projection;
}
