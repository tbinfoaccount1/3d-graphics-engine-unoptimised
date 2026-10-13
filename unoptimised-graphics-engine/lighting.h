#pragma once
#include<glm/glm.hpp>
#include<algorithm>


enum LightType
{
	Directional,
	Point
};

struct Light
{
	glm::vec3 coordinates{ -1.0f };
	glm::vec3 color{ 1.0f };
	float ambientStrength{ 0.15f };
	LightType type{ LightType::Directional };
};


float calculateDiffuse(const glm::vec3 normal, const glm::vec3 lightDirection);

float calculateSpecular(const glm::vec3 normal, const glm::vec3 lightDirection, const glm::vec3 viewDirection, const float shininess);