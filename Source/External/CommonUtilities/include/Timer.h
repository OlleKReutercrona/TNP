#pragma once

#include <chrono>

namespace CommonUtilities
{
	class Timer
	{
	public:
		Timer();
		Timer(const Timer& aTimer) = delete;
		Timer& operator=(const Timer& aTimer) = delete;

		void Update();

		float GetDeltaTime() const;
		double GetTotalTime() const;

	private:
		std::chrono::high_resolution_clock::time_point myLastFrameTime;
		std::chrono::high_resolution_clock::time_point myCurrentFrameTime;
		std::chrono::duration<float> myDeltaTime;
		std::chrono::duration<double> myTotalTime;
	};
}