#pragma once

#include <cstdint>
#include <glm/glm.hpp>

uint32_t packColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
uint32_t packColor(glm::vec4 color);
uint32_t packColor(float r, float g, float b, float a);

glm::vec4 unpackColor(uint32_t color);
glm::vec3 unpackColor3(uint32_t color);

uint32_t multiplyColors(uint32_t color1, uint32_t color2);
uint32_t multiplyColors(uint32_t color1, glm::vec4 color2);
uint32_t multiplyColors(uint32_t color1, glm::vec3 color2);

uint32_t blendColors(uint32_t color1, uint32_t color2, float alpha);