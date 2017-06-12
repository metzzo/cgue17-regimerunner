#pragma once
#include "glm/glm.hpp"
#include <SDL.h>
#include "Timer.h"

namespace Engine {

	Timer::Timer() {

	}

	Timer::Timer(bool looping, float interval)
	{
		loopState = looping;
		pauseState = false;
		durration = interval * 1000;

		if (interval < 0.0f)
			interval = 0.0f;

		elapsedTime = 0;
		startTime = GetProgramTime();
	}

	unsigned int Timer::GetProgramTime()
	{
		return SDL_GetTicks();
	}

	unsigned int Timer::GetTotalTime()
	{
		unsigned int deltaTime;

		if (pauseState)
			deltaTime = 0;
		else
			deltaTime = GetProgramTime() - startTime;

		return deltaTime + elapsedTime;
	}

	void Timer::PauseTimer()
	{
		pauseState = true;

		elapsedTime += GetProgramTime() - startTime;
	}

	void Timer::UnpauseTimer()
	{
		pauseState = false;

		startTime = GetProgramTime();
	}

	float Timer::GetElapsed()
	{
		unsigned int totalTime = GetTotalTime();

		if (loopState)
		{
			if (durration == 0.0f)
				return 0.0f;

			unsigned int modTime = totalTime % durration;

			return modTime / 1000.0f;
		}
		else
		{
			return totalTime / 1000.0f;
		}
	}

	float Timer::GetAlpha()
	{
		if (durration == 0)
			return -1.0f;

		float alpha;
		unsigned int totalTime = GetTotalTime();

		if (loopState)
		{
			unsigned int modTime = totalTime % durration;
			alpha = modTime / (float)durration;
		}
		else
		{
			alpha = totalTime / (float)durration;
		}

		return glm::clamp(alpha, 0.0f, (float)durration);
	}

	float Timer::TimeLeft()
	{
		unsigned int totalTime = GetTotalTime();
		if (durration == 0)
			return totalTime / -1000.0f;

		if (loopState)
		{
			unsigned int modTime = totalTime % durration;
			return (durration - modTime) / 1000.0f;
		}
		else
		{
			return (durration - totalTime) / 1000.0f;
		}
	}

	bool Timer::IsPassed() { return GetTotalTime() > durration; }

	bool Timer::IsPaused() { return pauseState; }

	void Timer::Reset()
	{
		pauseState = false;
		elapsedTime = 0;
		startTime = GetProgramTime();
	}
}