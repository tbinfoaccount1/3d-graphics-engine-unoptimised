#include<algorithm>

#include "color.h"
#include "transparency.h"

TransparencyBuffer::TransparencyBuffer(int width, int height) : width(width), height(height), pixels(width * height) {}

void TransparencyBuffer::clear() {
	for (auto& pixel : pixels) {
		pixel.clear();
	}
}

void  TransparencyBuffer::addFragment(int x, int y, float depth, uint32_t color, float opacity) {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		return;
	}
	pixels[y * width + x].push_back({ depth, color, opacity });
}

void TransparencyBuffer::renderTransparentPixels(FrameBuffer& framebuffer) {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			std::vector<TransparentFragment> fragments = pixels[y * width + x];
			if (fragments.empty()) {
				continue;
			}

			std::sort(fragments.begin(), fragments.end(), [](const TransparentFragment& a, const TransparentFragment& b) { return a.depth > b.depth; });

			uint32_t finalColor = framebuffer.pixels[y * width + x];

			for (const TransparentFragment& fragment : fragments) {
				if (fragment.depth > framebuffer.depthBuffer[y * width + x]) {
					continue;
				}
				finalColor = blendColors(finalColor, fragment.color, fragment.opacity);
			}

			framebuffer.pixels[y * width + x] = finalColor;
		}
	}

	clear();
}