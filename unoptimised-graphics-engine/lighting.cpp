#include "lighting.h"

float calculateDiffuse(const glm::vec3 normal, const glm::vec3 lightDirection)
{
	glm::vec3 normalizedNormal = glm::normalize(normal);
	glm::vec3 normalizedLight = glm::normalize(lightDirection);

	return std::max(glm::dot(normalizedNormal,normalizedLight), 0.0f);
}

float calculateSpecular(const glm::vec3 normal, const glm::vec3 lightDirection, const glm::vec3 viewDirection, const float shininess)
{
	glm::vec3 N = glm::normalize(normal);
	glm::vec3 L = glm::normalize(lightDirection);
	glm::vec3 V = glm::normalize(viewDirection);

	glm::vec3 H = glm::normalize(L + V);

	return std::pow(std::max(0.0f, glm::dot(N, H)), std::max(shininess, 1.0f));
}