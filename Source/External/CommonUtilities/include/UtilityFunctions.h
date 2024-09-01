#pragma once

namespace CommonUtilities
{
	template <typename T>
	T Min(const T& aFirst, const T& aSecond)
	{
		if(aFirst < aSecond)
		{
			return aFirst;
		}
		return aSecond;
	}

	// max
	template <typename T>
	T Max(const T& aFirst, const T& aSecond)
	{
		if(aFirst > aSecond)
		{
			return aFirst;
		}
		return aSecond;
	}

	// abs
	template <typename T>
	T Abs(const T& aValue)
	{
		if(aValue < 0)
		{
			return aValue * -1;
		}
		return aValue;
	}

	// clamp
	template <typename T>
	T Clamp(const T& aValue, const T& aMin, const T& aMax)
	{
		if(aValue < aMin)
		{
			return aMin;
		}
		else if(aValue > aMax)
		{
			return aMax;
		}
		return aValue;
	}

	// lerp
	template <typename T>
	T Lerp(const T& aFirst, const T& aSecond, const float& aValue)
	{
		return (aFirst + aValue * (aSecond - aFirst));
	}

	// swap
	template <typename T>
	void Swap(T& aFirst, T& aSecond)
	{
		T temp = aFirst;
		aFirst = aSecond;
		aSecond = temp;
	}
}