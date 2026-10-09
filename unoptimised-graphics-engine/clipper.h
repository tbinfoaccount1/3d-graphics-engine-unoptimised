#pragma once
#include <vector>
#include <array>
#include <glm/glm.hpp>

struct ClipVertex
{
	glm::vec4 position;
	glm::vec2 uv;
};

std::vector<std::array<ClipVertex, 3>> clipTriangle(const ClipVertex v0, const ClipVertex v1, const ClipVertex v2);