#include "lighting.h"

float calculateDiffuse(const glm::vec3 normal, const DirectionalLight& light)
{
	glm::vec3 normalizedNormal = glm::normalize(normal);
	glm::vec3 normalizedInverseLight = glm::normalize(-light.direction);

	return std::max(glm::dot(normalizedNormal,normalizedInverseLight), 0.0f);
}