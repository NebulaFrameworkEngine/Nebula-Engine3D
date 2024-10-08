#include "Utility.h"

#include <chrono>
#include <thread>

void LimitFPS(int targetFPS) {
	static auto lastFrameTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();

	// Calculate the time each frame should take to hit the target FPS
	float targetFrameDuration = 1.0f / targetFPS;

	// Get the duration of the last frame
	std::chrono::duration<float> frameTime = currentTime - lastFrameTime;

	// If the frame was rendered faster than the target time, sleep to cap FPS
	if (frameTime.count() < targetFrameDuration) {
		auto sleepDuration = std::chrono::duration<float>(targetFrameDuration - frameTime.count());
		std::this_thread::sleep_for(sleepDuration);
	}

	// Update lastFrameTime to the current time after sleeping
	lastFrameTime = std::chrono::high_resolution_clock::now();
}