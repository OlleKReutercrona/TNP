#pragma once

namespace CommonUtilities
{
	template <class T>
	class Vector4
	{
	public:
		T x;
		T y;
		T z;
		T w;
		//Creates a null-vector
		Vector4<T>();
		//Creates a vector (aX, aY, aZ)
		Vector4<T>(const T& aX, const T& aY, const T& aZ, const T& aW);
		//Copy constructor (compiler generated)
		Vector4<T>(const Vector4<T>& aVector) = default;
		//Assignment operator (compiler generated)
		Vector4<T>& operator=(const Vector4<T>& aVector4) = default;
		//Destructor (compiler generated)
		~Vector4<T>() = default;
		//Returns the squared length of the vector
		T LengthSqr() const;
		//Returns the length of the vector
		T Length() const;
		//Returns a normalized copy of this
		Vector4<T> GetNormalized() const;
		//Normalizes the vector
		void Normalize();
		//Returns the dot product of this and aVector
		T Dot(const Vector4<T>& aVector) const;

		inline const static Vector4<T> Up() { return { 0,	1,	0 } };
		inline const static Vector4<T> Right() { return { 1,	0,	0 } };
		inline const static Vector4<T> Left() { return { -1,	0,	0 } };
		inline const static Vector4<T> Down() { return { 0,	-1,	0 } };
		inline const static Vector4<T> Forward() { return { 0,	0,	1 } };
		inline const static Vector4<T> Back() { return { 0,	0, -1 } };
	};


	template<class T>
	inline Vector4<T>::Vector4()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	template<class T>
	inline Vector4<T>::Vector4(const T& aX, const T& aY, const T& aZ, const T& aW)
	{
		x = aX;
		y = aY;
		z = aZ;
		w = aW;
	}
	template<class T>
	inline T Vector4<T>::LengthSqr() const
	{
		return x * x + y * y + z * z + w * w;
	}
	template<class T>
	inline T Vector4<T>::Length() const
	{
		return sqrt(LengthSqr());
	}
	template<class T>
	inline Vector4<T> Vector4<T>::GetNormalized() const
	{
		Vector4<T> vec(x, y, z, w);
		vec.Normalize();
		return vec;
	}
	template<class T>
	inline void Vector4<T>::Normalize()
	{
		Vector4<T> vec(x, y, z, w);
		if(x + y + z + w != 0)
		{
			vec = vec / Length();
			x = vec.x;
			y = vec.y;
			z = vec.z;
			w = vec.w;
		}
	}
	template<class T>
	inline T Vector4<T>::Dot(const Vector4<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z + w * aVector.w;
	}

	//Returns the vector sum of aVector0 and aVector1
	template <class T> Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>&
		aVector1)
	{
		Vector4<T> tempVec = {
		aVector0.x + aVector1.x,
		aVector0.y + aVector1.y,
		aVector0.z + aVector1.z,
		aVector0.w + aVector1.w
		};
		return tempVec;
	}
	//Returns the vector difference of aVector0 and aVector1
	template <class T> Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>&
		aVector1)
	{
		Vector4<T> tempVec = {
		aVector0.x - aVector1.x,
		aVector0.y - aVector1.y,
		aVector0.z - aVector1.z,
		aVector0.w - aVector1.w
		};
		return tempVec;
	}
	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar)
	{
		Vector4<T> tempVec = {
		aVector.x * aScalar,
		aVector.y * aScalar,
		aVector.z * aScalar,
		aVector.w * aScalar
		};
		return tempVec;
	}
	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector)
	{
		Vector4<T> tempVec = {
		aVector.x * aScalar,
		aVector.y * aScalar,
		aVector.z * aScalar,
		aVector.w * aScalar
		};
		return tempVec;
	}
	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVectormultiplied by 1 / aScalar)
	template <class T> Vector4<T> operator/(const Vector4<T>& aVector, const T& aScalar)
	{
		Vector4<T> tempVec = {
		aVector.x / aScalar,
		aVector.y / aScalar,
		aVector.z / aScalar,
		aVector.w / aScalar,
		};
		return tempVec;
	}
	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <class T> void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
		aVector0.w += aVector1.w;
	}
	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class T> void operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
		aVector0.w -= aVector1.w;
	}
	//Equivalent to setting aVector to (aVector * aScalar)
	template <class T> void operator*=(Vector4<T>& aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
		aVector.w *= aScalar;
	}
	//Equivalent to setting aVector to (aVector / aScalar)
	template <class T> void operator/=(Vector4<T>& aVector, const T& aScalar)
	{
		aVector.x /= aScalar;
		aVector.y /= aScalar;
		aVector.z /= aScalar;
		aVector.w /= aScalar;
	}
}
