#pragma once
#include <array>

#define NUM_OF_OBJECTS 5

namespace TNP
{
	template<class T>
	class CircularBuffer
	{
	public:
		CircularBuffer();
		~CircularBuffer();

		inline T* GetLatest() { return myBuffer[myLatestIndex]; }
		inline std::array<T*, NUM_OF_OBJECTS>& GetAll() { return myBuffer; }
		void Add(T* aObject);

	private:
		std::array<T*, NUM_OF_OBJECTS> myBuffer = { nullptr };
		int myLatestIndex = 0;
	};


	template<class T>
	inline CircularBuffer<T>::CircularBuffer()
	{
	}

	template<class T>
	inline CircularBuffer<T>::~CircularBuffer()
	{
	}

	template<class T>
	inline void CircularBuffer<T>::Add(T* aObject)
	{
		// increment index
		myLatestIndex = ++myLatestIndex >= NUM_OF_OBJECTS ? 0 : myLatestIndex;

		if (myBuffer[myLatestIndex])
		{
			delete myBuffer[myLatestIndex];
		}

		myBuffer[myLatestIndex] = aObject;
	}
}

