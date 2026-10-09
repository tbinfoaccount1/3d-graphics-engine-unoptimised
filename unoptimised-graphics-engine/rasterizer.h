#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "objLoader.h"

struct RasterizerVertex
{
	glm::vec2 position;
	glm::vec2 uv;
	float invW{ 1.0f };
	float depth{ 0.0f };
};

struct FrameBuffer
{
	int width{ 0 };
	int height{ 0 };
	std::vector<uint32_t> pixels;
	std::vector<float> depthBuffer;
	
	FrameBuffer(int w, int h) : width(w), height(h), pixels(w* h, 0), depthBuffer(w * h, 1.0f) {};
};

float cross2D(const glm::vec2& a, const glm::vec2& b);

void clearFrameBuffer(FrameBuffer& framebuffer, uint32_t color);

void drawTriangle(FrameBuffer& frameBuffer, RasterizerVertex v0, RasterizerVertex v1, RasterizerVertex v2, Material& material);