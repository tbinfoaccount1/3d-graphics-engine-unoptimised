#pragma once

#include<cstdint>
#include<glm/glm.hpp>

uint32_t packColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return (static_cast<uint32_t>(a) << 24) |
		(static_cast<uint32_t>(b) << 16) |
		(static_cast<uint32_t>(g) << 8) |
		static_cast<uint32_t>(r);
}

uint32_t packColor(glm::vec4 color)
{
	return (static_cast<uint32_t>(color.a * 255.0f) << 24) |
		(static_cast<uint32_t>(color.b * 255.0f) << 16) |
		(static_cast<uint32_t>(color.g * 255.0f) << 8) |
		static_cast<uint32_t>(color.r * 255.0f);
}

uint32_t packColor(float r, float g, float b, float a)
{
	return (static_cast<uint32_t>(a * 255.0f) << 24) |
		(static_cast<uint32_t>(b * 255.0f) << 16) |
		(static_cast<uint32_t>(g * 255.0f) << 8) |
		static_cast<uint32_t>(r * 255.0f);
}

glm::vec4 unpackColor(uint32_t color)
{
	uint8_t r = static_cast<uint8_t>(color & 0xFF);
	uint8_t g = static_cast<uint8_t>((color >> 8) & 0xFF);
	uint8_t b = static_cast<uint8_t>((color >> 16) & 0xFF);
	uint8_t a = static_cast<uint8_t>((color >> 24) & 0xFF);
	return glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

uint32_t multiplyColors(uint32_t color1, uint32_t color2)
{
	glm::vec4 c1 = unpackColor(color1);
	glm::vec4 c2 = unpackColor(color2);
	glm::vec4 result = c1 * c2;
	return packColor(result);
}

uint32_t multiplyColors(uint32_t color1, glm::vec4 color2)
{
	glm::vec4 c1 = unpackColor(color1);
	glm::vec4 result = c1 * color2;
	return packColor(result);
}

uint32_t multiplyColors(uint32_t color1, glm::vec3 color2)
{
	glm::vec4 c1 = unpackColor(color1);
	glm::vec4 result = c1 * glm::vec4(color2, 1.0f);
	return packColor(result);
}