#pragma once
#include "Vector.h"

namespace CommonUtilities
{
	using namespace CommonUtilities;

	template <class T>
	class Plane
	{
	public:
		// Default constructor.
		Plane() = default;
		
		// Constructor taking three points where the normal is (aPoint1 - aPoint0) x (aPoint2-aPoint0).
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		
		// Constructor taking a point and a normal.
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal);
		
		// Init the plane with three points, the same as the constructor above.
		void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		
		// Init the plane with a point and a normal, the same as the constructor above.
		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);
		
		// Returns whether a point is inside the plane: it is inside when the point is on the
		// plane or on the side the normal is pointing away from.
		bool IsInside(const Vector3<T>&aPosition) const;

		bool IsOnPlane(const Vector3<T>& aPosition) const;
		
		// Returns the normal of the plane.
		const Vector3<T>& GetNormal() const;

		const Vector3<T>& GetPoint0() const;
	private:
		Vector3<T> myPoint0;
		Vector3<T> myPoint1;
		Vector3<T> myPoint2;

		Vector3<T> myNormal;
	};

	template<class T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint0 = aPoint0;
		myPoint1 = aPoint1;
		myPoint2 = aPoint2;

		Vector3<T> tempVec1 = myPoint1 - myPoint0;
		Vector3<T> tempVec2 = myPoint2 - myPoint0;

		myNormal = tempVec1.Cross(tempVec2);
	}

	template<class T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal)
	{
		myPoint0 = aPoint0;
		myNormal = aNormal;
	}

	template<class T>
	inline void Plane<T>::InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint0 = aPoint0;
		myPoint1 = aPoint1;
		myPoint2 = aPoint2;

		Vector3<T> tempVec1 = myPoint1 - myPoint0;
		Vector3<T> tempVec2 = myPoint2 - myPoint0;

		myNormal = tempVec1.Cross(tempVec2);
	}

	template<class T>
	inline void Plane<T>::InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		myPoint0 = aPoint;
		myNormal = aNormal;
	}

	template<class T>
	inline bool Plane<T>::IsInside(const Vector3<T>& aPosition) const
	{
		Vector3<T> pointToTest = aPosition - myPoint0;

		return pointToTest.Dot(myNormal) < 0.000001f;
	}

	template<class T>
	inline bool Plane<T>::IsOnPlane(const Vector3<T>& aPosition) const
	{
		Vector3<T> pointToTest = aPosition - myPoint0;

		return pointToTest.Dot(myNormal) < 0.000001f && pointToTest.Dot(myNormal) > -0.000001f;
	}

	template<class T>
	inline const Vector3<T>& Plane<T>::GetNormal() const
	{
		return myNormal;
	}
	template<class T>
	inline const Vector3<T>& Plane<T>::GetPoint0() const
	{
		return myPoint0;
	}
}