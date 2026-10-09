#include "rasterizer.h"
#include "clipper.h"
#include "color.h"

float cross2D(const glm::vec2& a, const glm::vec2& b)
{
	return a.x * b.y - a.y * b.x;
}

void clearFrameBuffer(FrameBuffer& framebuffer, uint32_t color)
{
	std::fill(framebuffer.pixels.begin(), framebuffer.pixels.end(), color);
	std::fill(framebuffer.depthBuffer.begin(), framebuffer.depthBuffer.end(), 1.0f);
}

uint32_t sampleTexture(const Texture& texture, const glm::vec2& uv)
{
	if (texture.isEmpty())
	{
		return 0xFFFFFFFF;
	}
	int x = static_cast<int>(uv.x * texture.width) % texture.width;
	int y = static_cast<int>(uv.y * texture.height) % texture.height;
	if (x < 0) x += texture.width;
	if (y < 0) y += texture.height;
	return texture.pixels[y * texture.width + x];
}

void drawTriangle(FrameBuffer& frameBuffer, RasterizerVertex v0, RasterizerVertex v1, RasterizerVertex v2, Material& material)
{
	glm::vec2 v0v1 = v1.position - v0.position;
	glm::vec2 v1v2 = v2.position - v1.position;
	glm::vec2 v2v0 = v0.position - v2.position;

	float area = cross2D(v0v1, v2.position - v0.position);

	for (int y = 0; y < frameBuffer.height; ++y)
	{
		for (int x = 0; x < frameBuffer.width; ++x)
		{
			glm::vec2 p(x + 0.5f, y + 0.5f);

			float w0 = cross2D(v1v2, p - v1.position);
			float w1 = cross2D(v2v0, p - v2.position);
			float w2 = cross2D(v0v1, p - v0.position);

			if (!((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)))
			{
				continue;
			}

			float a = cross2D(v1.position - p, v2.position - p) / area;
			float b = cross2D(v2.position - p, v0.position - p) / area;
			float c = cross2D(v0.position - p, v1.position - p) / area;

			float depth = a * v0.depth + b * v1.depth + c * v2.depth;

			if (depth > frameBuffer.depthBuffer[y * frameBuffer.width + x]) 
			{
				continue;
			}

			float denominator = a * v0.invW + b * v1.invW + c * v2.invW;
			if (denominator == 0.0f)
			{
				continue;
			}

			glm::vec2 uv = (a * v0.uv * v0.invW + b * v1.uv * v1.invW + c * v2.uv * v2.invW) / denominator;

			uint32_t color = sampleTexture(material.diffuseTexture, uv);

			frameBuffer.pixels[y * frameBuffer.width + x] = multiplyColors(color, material.diffuseColor);
			frameBuffer.depthBuffer[y * frameBuffer.width + x] = depth;
			
		}
	}
}