#include "rasterizer.h"
#include "clipper.h"
#include "color.h"
#include "transparency.h"

#include<iostream>

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

void drawTriangle(FrameBuffer& frameBuffer, RasterizerVertex v0, RasterizerVertex v1, RasterizerVertex v2, Material& material, Light light, glm::vec3 camPos)
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
			glm::vec3 normal = glm::normalize((a * v0.normal * v0.invW + b * v1.normal * v1.invW + c * v2.normal * v2.invW) / denominator);
			glm::vec3 worldPos = (a * v0.worldPos * v0.invW + b * v1.worldPos * v1.invW + c * v2.worldPos * v2.invW) / denominator;
			glm::vec3 tangent = (a * v0.tangent * v0.invW + b * v1.tangent * v1.invW + c * v2.tangent * v2.invW) / denominator;
			glm::vec3 bitangent = (a * v0.bitangent * v0.invW + b * v1.bitangent * v1.invW + c * v2.bitangent * v2.invW) / denominator;

			tangent -= normal * glm::dot(normal, tangent);
			float tangentLength = glm::length(tangent);
			if (tangentLength > 0.000001f)
			{
				tangent /= tangentLength;
				glm::vec3 B = glm::cross(normal, tangent);
				if (glm::dot(B, bitangent) < 0.0f)
					B = -B;
				bitangent = B;
			}

			glm::vec2 texelSize(1.0f / (float)std::max(1, material.bumpTexture.width), 1.0f / (float)std::max(1, material.bumpTexture.height));
			float heightL = unpackColor(sampleTexture(material.bumpTexture, uv - glm::vec2(texelSize.x, 0.0f))).r;
			float heightR = unpackColor(sampleTexture(material.bumpTexture, uv + glm::vec2(texelSize.x, 0.0f))).r;
			float heightD = unpackColor(sampleTexture(material.bumpTexture, uv - glm::vec2(0.0f, texelSize.y))).r;
			float heightU = unpackColor(sampleTexture(material.bumpTexture, uv + glm::vec2(0.0f, texelSize.y))).r;
			float dHeightU = (heightR - heightL) * 0.5f;
			float dHeightV = (heightU - heightD) * 0.5f;

			float bumpScale = 1.5f;
			glm::vec3 pixelHeight = glm::normalize(glm::vec3(dHeightU * bumpScale, dHeightV * bumpScale, 1.0f));
			glm::mat3 TBN = glm::mat3(glm::normalize(tangent), glm::normalize(bitangent), normal);
			normal = glm::normalize(TBN * pixelHeight);

			uint32_t transparencyPixel = sampleTexture(material.transparencyTexture, uv);
			float opacity = unpackColor(transparencyPixel).r * material.transparency;

			glm::vec3 lightDirection = (light.type == LightType::Directional) ? light.coordinates : (worldPos - light.coordinates);
			float pointLightDistance{ 0.0f };
			float attenuation{ 1.0f };
			if (light.type == LightType::Point)
			{
				pointLightDistance = glm::length(lightDirection);
				attenuation = 1 / (light.constant + pointLightDistance * (light.linear + pointLightDistance * light.quadratic));
			}
			lightDirection = glm::normalize(lightDirection);

			glm::vec3 diffusePixelColor = material.diffuseColor * unpackColor3(sampleTexture(material.diffuseTexture, uv));
			glm::vec3 diffuseColor = attenuation * light.color * calculateDiffuse(normal, lightDirection) * diffusePixelColor;

			glm::vec3 ambientPixelColor = material.ambientColor * unpackColor3(sampleTexture(material.ambientTexture, uv));
			glm::vec3 ambientColor = light.ambientStrength * ambientPixelColor;

			glm::vec3 specularPixelColor = material.specularColor * unpackColor3(sampleTexture(material.specularTexture, uv));
			float totalShininess = material.shininess * unpackColor(sampleTexture(material.shininessTexture, uv)).r;
			glm::vec3 specularColor = attenuation * light.color * specularPixelColor * calculateSpecular(normal, lightDirection, worldPos - camPos, totalShininess);

			glm::vec3 lighting = diffuseColor + ambientColor + specularColor;
			uint32_t color = packColor(glm::vec4(glm::clamp(lighting, glm::vec3(0.0f), glm::vec3(1.0f)), 1.0f));
			
			if (opacity >= 1.0f)
			{
				frameBuffer.pixels[y * frameBuffer.width + x] = color;
				frameBuffer.depthBuffer[y * frameBuffer.width + x] = depth;
			}
			else if (opacity > 0.0f)
			{
				extern TransparencyBuffer transparencyBuffer;
				transparencyBuffer.addFragment(x, y, depth, color, opacity);
			}
		}
	}
}