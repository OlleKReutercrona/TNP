#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

#include <cassert>
#include <array>
#include <cmath>

namespace CommonUtilities
{
	template <class T>
	class Matrix3x3
	{
	public:
		// Creates the identity matrix. 
		Matrix3x3<T>();

		// Copy Constructor.
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix);

		// Copies the top left 3x3 part of the Matrix4x4.
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);

		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		Matrix3x3<T>& operator=(const Matrix3x3<T> aMatrix);

		// Static functions for creating rotation matrices.
		static Matrix3x3<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(T aAngleInRadians);

		// Static function for creating a transpose of a matrix.
		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);

		const static int mySize = 3;
		std::array<std::array<T, mySize>, mySize> myMatrix;
	private:
		template<class T> friend class Matrix4x4;
	};

	// Copy Constructor.
	template<class T>
	inline Matrix3x3<T>::Matrix3x3()
	{
		for(size_t y = 0; y < mySize; y++)
		{
			for(size_t x = 0; x < mySize; x++)
			{
				myMatrix[x][y] = 0;
			}
		}
		for(size_t i = 0; i < mySize; i++)
		{
			myMatrix[i][i] = 1;
		}
	}


	// Copy Constructor.
	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix3x3<T>& aMatrix)
	{
		myMatrix = aMatrix.myMatrix;
	}

	// Copies the top left 3x3 part of the Matrix4x4.
	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
	{
		for (size_t i = 0; i < mySize; i++)
		{
			for(size_t j = 0; j < mySize; j++)
			{
				myMatrix[j][i] = aMatrix(j+1, i+1);
			}
		}
	}

	// () operator for accessing element (row, column) for read/write or read, respectively.
	template<class T>
	inline T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		assert(aRow > 0 && "Row index is negative");
		assert(aColumn > 0 && "Column index is negative");
		assert(aRow <= mySize && "Row index is out of bounds");
		assert(aColumn <= mySize && "Column index is out of bounds");
		
		return myMatrix[aRow-1][aColumn-1];
	}

	template<class T>
	inline const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		assert(aRow > 0 && "Row index is negative");
		assert(aColumn > 0 && "Column index is negative");
		assert(aRow <= mySize && "Row index is out of bounds");
		assert(aColumn <= mySize && "Column index is out of bounds");

		return myMatrix[aRow-1][aColumn-1];
	}

	template<class T>
	inline const Matrix3x3<T> operator+(const Matrix3x3<T>& aFirstMatrix, const Matrix3x3<T>& aSecondMatrix)
	{
		Matrix3x3<T> tempMatrix;

		for (size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for (size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				tempMatrix(j, i) = aFirstMatrix(j, i) + aSecondMatrix(j, i);
			}
		}

		return tempMatrix;
	}

	template<class T>
	inline void operator+=(Matrix3x3<T>& aFirstMatrix, const Matrix3x3<T> aSecondMatrix)
	{
		for (size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for (size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				aFirstMatrix(j, i) = aFirstMatrix(j, i) + aSecondMatrix(j, i);
			}
		}
	}

	template<class T>
	inline const Matrix3x3<T> operator-(Matrix3x3<T>& aFirstMatrix, const Matrix3x3<T>& aSecondMatrx)
	{
		Matrix3x3<T> tempMatrix;

		for (size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for (size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				tempMatrix(i, j) = aFirstMatrix(i, j) - aSecondMatrx(i, j);
			}
		}

		return tempMatrix;
	}

	template<class T>
	inline void operator-=(Matrix3x3<T>& aFirstMatrix, const Matrix3x3<T> aSecondMatrix)
	{
		Matrix3x3<T> tempMatrix;
		for (size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for (size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				tempMatrix(i, j) = aFirstMatrix(i, j) - aSecondMatrix(i, j);
			}
		}
		aFirstMatrix = tempMatrix;
	}

	template<class T>
	inline const Matrix3x3<T> operator*(const Matrix3x3<T>& aFirstMatrix, const Matrix3x3<T> aSecondMatrix)
	{
		Matrix3x3<T> tempMatrix;

		for (size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for (size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				T a = aFirstMatrix(i, 1);
				T b = aFirstMatrix(i, 2);
				T c	= aFirstMatrix(i, 3);
				T r	= aSecondMatrix(1, j);
				T s	= aSecondMatrix(2, j);
				T t	= aSecondMatrix(3, j);

				tempMatrix(i, j) = a*r + b*s + c*t;
			}
		}

		return tempMatrix;
	}

	template<class T>
	inline void operator*=(Matrix3x3<T>& aFirstMatrix, const Matrix3x3<T> aSecondMatrix)
	{
		Matrix3x3<T> tempMatrix;

		for (size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for (size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				T a = aFirstMatrix(i, 1);
				T b = aFirstMatrix(i, 2);
				T c = aFirstMatrix(i, 3);
				T n = aSecondMatrix(1, j);
				T q = aSecondMatrix(2, j);
				T t = aSecondMatrix(3, j);

				tempMatrix(i, j) = a * n + b * q + c * t;
			}
		}
		aFirstMatrix = tempMatrix;
	}

	template<class T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator=(const Matrix3x3<T> aMatrix)
	{
		for (size_t i = 0; i < mySize; i++)
		{
			for (size_t j = 0; j < mySize; j++)
			{
				myMatrix[j][i] = aMatrix(j+1, i+1);
			}
		}
		return *this;
	}

	template<class T>
	inline const bool operator==(const Matrix3x3<T> aFirstMatrix, const Matrix3x3<T> aSecondMatrix)
	{
		for (size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for (size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				if (aFirstMatrix(j,i) != aSecondMatrix(j, i))
				{
					return false;
				}
			}
		}
		return true;
	}

	template<class T>
	inline const bool operator!=(const Matrix3x3<T> aFirstMatrix, const Matrix3x3<T> aSecondMatrix)
	{
		bool isEqual = true;

		for (size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for (size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				if (aFirstMatrix(j, i) != aSecondMatrix(j, i))
				{
					isEqual = false;
				}
			}
		}
		return isEqual;
	}

	template<class T>
	inline const Vector3<T> operator*(const Vector3<T>& aVector, const Matrix3x3<T>& aMatrix)
	{
		Vector3<T> tempVec;

		tempVec.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1);
		tempVec.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2);
		tempVec.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3);

		return tempVec;
	}

	template<class T>
	inline const Vector3<T> operator*(const Matrix3x3<T>& aMatrix, const Vector3<T>& aVector)
	{
		Vector3<T> tempVec;

		tempVec.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1);
		tempVec.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2);
		tempVec.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3);

		return tempVec;
	}

	// Static functions for creating rotation matrices.
	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix3x3<T> tempMatrix;

		tempMatrix(1, 1) = 1;
		tempMatrix(2, 2) = cos(aAngleInRadians);
		tempMatrix(2, 3) = sin(aAngleInRadians);
		tempMatrix(3, 2) = -sin(aAngleInRadians);
		tempMatrix(3, 3) = cos(aAngleInRadians);

		return tempMatrix;
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix3x3<T> tempMatrix;

		tempMatrix(2, 2) = 1;
		tempMatrix(1, 1) = cos(aAngleInRadians);
		tempMatrix(1, 3) = -sin(aAngleInRadians);
		tempMatrix(3, 1) = sin(aAngleInRadians);
		tempMatrix(3, 3) = cos(aAngleInRadians);

		return tempMatrix;
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix3x3<T> tempMatrix;

		tempMatrix(3, 3) = 1;
		tempMatrix(1, 1) = cos(aAngleInRadians);
		tempMatrix(1, 2) = sin(aAngleInRadians);
		tempMatrix(2, 1) = -sin(aAngleInRadians);
		tempMatrix(2, 2) = cos(aAngleInRadians);

		return tempMatrix;
	}

	// Static function for creating a transpose of a matrix.
	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::Transpose(const Matrix3x3<T>& aMatrixToTranspose)
	{
		Matrix3x3<T> tempMatrix;

		for (size_t i = 1; i < mySize + 1; i++)
		{
			for (size_t j = 1; j < mySize + 1; j++)
			{
				tempMatrix(j, i) = aMatrixToTranspose(i, j);
			}
		}

		return tempMatrix;
	}

}