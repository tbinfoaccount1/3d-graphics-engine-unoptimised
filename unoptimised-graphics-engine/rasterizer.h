#pragma once
#include <vector>
#include <glm/glm.hpp>


struct FrameBuffer
{
	int width{ 0 };
	int height{ 0 };
	std::vector<uint32_t> pixels;
	
	FrameBuffer(int w, int h) : width(w), height(h), pixels(w* h, 0) {};
};

float cross2D(const glm::vec2& a, const glm::vec2& b);

void clearFrameBuffer(FrameBuffer& framebuffer, uint32_t color);

void drawTriangle(FrameBuffer& frameBuffer, glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, uint32_t color);