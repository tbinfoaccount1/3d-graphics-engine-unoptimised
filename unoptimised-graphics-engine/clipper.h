#pragma once
#include <vector>
#include <array>
#include <glm/glm.hpp>

std::vector<std::array<glm::vec4, 3>> clipTriangle(const glm::vec4 v0, const glm::vec4 v1, const glm::vec4 v2);