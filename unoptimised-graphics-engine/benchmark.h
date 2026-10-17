#pragma once

class Benchmark
{
public:
	Benchmark();

	void startMeasure();

	void pauseMeasure();

	void resumeMeasure();

	void stopMeasure();

	void handleFrame();

private:
	Uint64 totalFrames{ 0 };
	Uint64 startTime{ 0 };
	bool measuring{ false };
	Uint64 pauseStartTime{ 0 };
	Uint64 totalPausedTime{ 0 };
};