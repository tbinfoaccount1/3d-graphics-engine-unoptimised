#include <SDL3/SDL.h>
#include <iostream>
#include "benchmark.h"

Benchmark::Benchmark() {};

void Benchmark::startMeasure()
{
	totalFrames = 0;
	startTime = SDL_GetTicksNS();
	measuring = true;
}

void Benchmark::pauseMeasure()
{
	pauseStartTime = SDL_GetTicksNS();
	measuring = false;
}

void Benchmark::resumeMeasure()
{
	Uint64 pausedTime = SDL_GetTicksNS() - pauseStartTime;
	totalPausedTime += pausedTime;
	measuring = true;
}

void Benchmark::stopMeasure()
{
	Uint64 deltaTime = SDL_GetTicksNS() - startTime - totalPausedTime;

	Uint64 timePerFrame = deltaTime / totalFrames;

	float fps = (1000000000.0f / timePerFrame);

	std::cout << deltaTime << " --- " << startTime << " --- " << totalFrames << " --- " << timePerFrame << std::endl;

	std::cout << "\n***** BENCHMARK *****\n  FPS: " << fps << "\n*********************\n\n";
}

void Benchmark::handleFrame()
{
	totalFrames++;
}