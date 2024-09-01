#pragma once
#include "Plane.h"
#include "AABB3D.h"
#include "Ray.h"
#include "Vector3.h"
#include "Sphere.h"
#include <cmath>

namespace CommonUtilities
{
	template<typename T>
	inline bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		Vector3<T> Po = aRay.GetOrigin();
		Vector3<T> N = aPlane.GetNormal();
		T d = aPlane.GetPoint0().Dot(N);
		Vector3<T> D = aRay.GetDirection();

		T t = (d - Po.Dot(N)) / 
				(D.Dot(N));

		if(aPlane.IsOnPlane(Po)) // Is on plane == Is intersecting
		{
			aOutIntersectionPoint = Po + D * t;
			return true;
		}

		if(D.Dot(N) == 0) // Ray is parallel
		{
			return false;
		}

		if(D.Dot(N) < 0) // Ray is negative (backwards)
		{
			return false;
		}

		aOutIntersectionPoint = Po + D * t;
		return true;
	}

	template<typename T>
	inline bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		const Vector3<T> rayOrg = aRay.GetOrigin();
		const Vector3<T> rayDelta = aRay.GetDirection();
		const Vector3<T> min = aAABB.GetMin();
		const Vector3<T> max = aAABB.GetMax();

		T xout, yout, zout;
		if(!IsInside(rayOrg.x, rayDelta.x, min.x, max.x, xout)) return false;
		if(!IsInside(rayOrg.y, rayDelta.y, min.y, max.y, yout)) return false;
		if(!IsInside(rayOrg.z, rayDelta.z, min.z, max.z, zout)) return false;

		// Select farthest plane - This is the plane of intersection.
		int which = 0;
		T t = xout;
		if(yout > t)
		{
			which = 1;
			t = yout;
		}
		if(zout > t)
		{
			which = 2;
			t = zout;
		}

		Vector3<T> outV = aRay.GetOrigin();
		// T x = T(), y = T(), z = T();

		switch(which)
		{
			case 0: // Intersect with yz plane
			{
				outV.y = rayOrg.y + rayDelta.y * t;
				if(outV.y < min.y || outV.y > max.y) return false;
				outV.z = rayOrg.z + rayDelta.z * t;
				if(outV.z < min.z || outV.z > max.z) return false;
			}
			break;
			case 1: // Intersect with xz plane
			{
				outV.x = rayOrg.x + rayDelta.x * t;
				if(outV.x < min.x || outV.x > max.x) return false;
				outV.z = rayOrg.z + rayDelta.z * t;
				if(outV.z < min.z || outV.z > max.z) return false;
			}
			break;
			case 2: // Intersect with xy plane
			{
				outV.x = rayOrg.x + rayDelta.x * t;
				if(outV.x < min.x || outV.x > max.x) return false;
				outV.y = rayOrg.y + rayDelta.y * t;
				if(outV.y < min.y || outV.y > max.y) return false;
			}
			break;
		}

		//aOutIntersectionPoint = Vector3<T>{ x, y, z };
		aOutIntersectionPoint = outV;
		return true;
	}

	template<typename T>
	bool IsInside(const T& aRayOriginValue, const T& aRayDeltaValue, const T& aMinValue, const T& aMaxValue, T& anOutValue)
	{
		if(aRayOriginValue < aMinValue)
		{
			anOutValue = aMinValue - aRayOriginValue;
			if(anOutValue > aRayDeltaValue) return false;
			anOutValue /= aRayDeltaValue;
		}
		else if(aRayOriginValue > aMaxValue)
		{
			anOutValue = aMaxValue - aRayOriginValue;
			if(aMaxValue - aRayOriginValue < aRayDeltaValue) return false;
			anOutValue /= aRayDeltaValue;
		}
		else
		{
			anOutValue = 1.0f;
		}
		return true;
	}

	template<typename T>
	inline bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		// See page 728 of 3D Math Primer for Graphics and Game Development

		Vector3<T> Po = aRay.GetOrigin(); // Ray origin
		Vector3<T> D = aRay.GetDirection(); // Ray direction
		Vector3<T> C = aSphere.GetPosition(); // Center of sphere
		Vector3<T> E = C - Po; // Vector from Ray origin to center of sphere
		T r = aSphere.GetRadius(); // Radius of sphere
		T a = E.Dot(D.GetNormalized()); // Distance from origin to center on the direction of the ray
		T e = Vector3<T>(C - Po).Length(); // Distance from origin to center
		T b = sqrt(e * e - a * a); // Distance from ray to center on rays distnace 
		T f = sqrt(r * r - e * e + a * a); // Length of distance from the collisionpoint to the center of sphere in the direction of ray
		T t = a - f; // Length of distance from collisionpoint to ray origin in the direction of ray

		if(e * e < r * r) // Ray Origin is inside sphere
		{
			aOutIntersectionPoint = Po + t * D.GetNormalized();
			return true;
		}

		if(r * r - e * e + a * a < 0) // Ray is not colliding
		{
			return false;
		}

		aOutIntersectionPoint = Po + t * D.GetNormalized();
		return true;
	}
}