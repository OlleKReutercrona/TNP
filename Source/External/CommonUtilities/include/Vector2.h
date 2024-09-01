#pragma once

namespace CommonUtilities
{
	template <class T>
	class Vector2
	{
	public:
		T x;
		T y;
		//Creates a null-vector
		Vector2<T>();
		//Creates a vector (aX, aY, aZ)
		Vector2<T>(const T& aX, const T& aY);
		//Copy constructor (compiler generated)
		Vector2<T>(const Vector2<T>& aVector) = default;
		//Assignment operator (compiler generated)
		Vector2<T>& operator=(const Vector2<T>& aVector2) = default;
		//Destructor (compiler generated)
		~Vector2<T>() = default;
		//Returns the squared length of the vector
		T LengthSqr() const;
		//Returns the length of the vector
		T Length() const;
		//Returns a normalized copy of this
		Vector2<T> GetNormalized() const;
		//Normalizes the vector
		void Normalize();
		//Returns the dot product of this and aVector
		T Dot(const Vector2<T>& aVector) const;

		inline const static Vector2<T> Up() { return { 0, 1 } };
		inline const static Vector2<T> Right() { return { 1, 0 } };
		inline const static Vector2<T> Left() { return { -1, 0 } };
		inline const static Vector2<T> Down() { return { 0,	-1 } };
	};


	template<class T>
	inline Vector2<T>::Vector2()
	{
		x = 0;
		y = 0;
	}
	template<class T>
	inline Vector2<T>::Vector2(const T& aX, const T& aY)
	{
		x = aX;
		y = aY;
	}
	template<class T>
	inline T Vector2<T>::LengthSqr() const
	{
		return x * x + y * y;
	}
	template<class T>
	inline T Vector2<T>::Length() const
	{
		return sqrt(LengthSqr());
	}
	template<class T>
	inline Vector2<T> Vector2<T>::GetNormalized() const
	{
		Vector2<T> vec(x, y);
		vec.Normalize();
		return vec;
	}
	template<class T>
	inline void Vector2<T>::Normalize()
	{
		Vector2<T> vec(x, y);
		if(x + y != 0)
		{
			vec = vec / Length();
			x = vec.x;
			y = vec.y;
		}
	}
	template<class T>
	inline T Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y;
	}

	//Returns the vector sum of aVector0 and aVector1
	template <class T> Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>&
		aVector1)
	{
		Vector2<T> tempVec = {
		aVector0.x + aVector1.x,
		aVector0.y + aVector1.y,
		};
		return tempVec;
	}
	//Returns the vector difference of aVector0 and aVector1
	template <class T> Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>&
		aVector1)
	{
		Vector2<T> tempVec = {
		aVector0.x - aVector1.x,
		aVector0.y - aVector1.y,
		};
		return tempVec;
	}
	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar)
	{
		Vector2<T> tempVec = {
		aVector.x * aScalar,
		aVector.y * aScalar,
		};
		return tempVec;
	}
	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
	{
		Vector2<T> tempVec = {
		aVector.x * aScalar,
		aVector.y * aScalar,
		};
		return tempVec;
	}
	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVectormultiplied by 1 / aScalar)
	template <class T> Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar)
	{
		Vector2<T> tempVec = {
		aVector.x / aScalar,
		aVector.y / aScalar,
		};
		return tempVec;
	}
	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <class T> void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
	}
	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class T> void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
	}
	//Equivalent to setting aVector to (aVector * aScalar)
	template <class T> void operator*=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
	}
	//Equivalent to setting aVector to (aVector / aScalar)
	template <class T> void operator/=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector.x /= aScalar;
		aVector.y /= aScalar;
	}
}
