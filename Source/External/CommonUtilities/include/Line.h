#pragma once
#include "Vector2.h"
#include <cmath>

namespace CommonUtilities
{
	using namespace CommonUtilities;

	template <class T>
	class Line
	{
	public:
		// Default constructor: there is no line, the normal is the zero vector.
		Line() = default;

		// Copy constructor.
		Line(const Line<T>& aLine);

		// Constructor that takes two points that define the line, the direction is aPoint1 - aPoint0.
		Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);

		// Init the line with two points, the same as the constructor above.
		void InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);
		
		// Init the line with a point and a direction.
		void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection);
		
		// Returns whether a point is inside the line: it is inside when the point is on the
		// line or on the side the normal is pointing away from.
		bool IsInside(const Vector2<T>& aPosition) const;

		// Returns the direction of the line.
		const Vector2<T>& GetDirection() const;

		// Returns the normal of the line, which is (-direction.y, direction.x).
		const Vector2<T>& GetNormal() const;
	private:
		Vector2<T> myPoint0;
		Vector2<T> myPoint1;

		Vector2<T> myNormal;
		Vector2<T> myDirection;
	};

	template<class T>
	inline Line<T>::Line(const Line<T>& aLine)
	{
		myPoint0 = aLine.myPoint0;
		myPoint1 = aLine.myPoint1;
		myNormal = aLine.myNormal;
		myDirection = aLine.myDirection;
	}

	template<class T>
	inline Line<T>::Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myPoint0 = aPoint0;
		myPoint1 = aPoint1;

		myDirection = aPoint1 - aPoint0;
		myNormal = { -myDirection.y, myDirection.x };
	}

	template<class T>
	inline void Line<T>::InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myPoint0 = aPoint0;
		myPoint1 = aPoint1;

		myDirection = aPoint1 - aPoint0;
		myNormal = { -myDirection.y, myDirection.x };
	}

	template<class T>
	inline void Line<T>::InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
	{
		myPoint0 = aPoint;

		myDirection = aDirection;
		myNormal = { -myDirection.y, myDirection.x };
	}

	template<class T>
	inline bool Line<T>::IsInside(const Vector2<T>& aPosition) const
	{
		Vector2<T> pointToTest = aPosition - myPoint0;

		return pointToTest.Dot(myNormal) < 0.000001f;
	}

	template<class T>
	inline const Vector2<T>& Line<T>::GetDirection() const
	{
		return myDirection;
	}

	template<class T>
	inline const Vector2<T>& Line<T>::GetNormal() const
	{
		return { -myDirection.y, myDirection.x };
	}
}