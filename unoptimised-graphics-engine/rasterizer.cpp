#include "rasterizer.h"

float cross2D(const glm::vec2& a, const glm::vec2& b)
{
	return a.x * b.y - a.y * b.x;
}

void clearFrameBuffer(FrameBuffer& framebuffer, uint32_t color)
{
	std::fill(framebuffer.pixels.begin(), framebuffer.pixels.end(), color);
}

void drawTriangle(FrameBuffer& frameBuffer, glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, uint32_t color)
{
	glm::vec2 v0v1 = v1 - v0;
	glm::vec2 v1v2 = v2 - v1;
	glm::vec2 v2v0 = v0 - v2;

	for (int y = 0; y < frameBuffer.height; ++y)
	{
		for (int x = 0; x < frameBuffer.width; ++x)
		{
			glm::vec2 p(x, y);

			if ((cross2D(v0v1, p - v0) >= 0 && cross2D(v1v2, p - v1) >= 0 && cross2D(v2v0, p - v2) >= 0) ||
				(cross2D(v0v1, p - v0) <= 0 && cross2D(v1v2, p - v1) <= 0 && cross2D(v2v0, p - v2) <= 0))
			{
				frameBuffer.pixels[y * frameBuffer.width + x] = color;
			}
		}
	}
}