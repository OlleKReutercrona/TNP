#pragma once
namespace TNP
{
	class Timer
	{
	public:
		Timer();
		~Timer();

		void StartTimer();
		float GetDeltaTime();
		void StopTimer();

	private:


	};
}

