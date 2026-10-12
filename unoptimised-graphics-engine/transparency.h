#pragma once

#include<vector>
#include "rasterizer.h"

struct TransparentFragment
{
    float depth;
    uint32_t color;
    float opacity;
};

class TransparencyBuffer
{
public:
    TransparencyBuffer(int width, int height);

    void clear();

    void addFragment(int x, int y, float depth, uint32_t color, float opacity);

    void renderTransparentPixels(FrameBuffer& framebuffer);

private:
    int width = 0;
    int height = 0;

    std::vector<std::vector<TransparentFragment>> pixels;
};