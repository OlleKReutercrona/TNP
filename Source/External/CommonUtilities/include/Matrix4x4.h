#pragma once
#include "Vector3.h"
#include "Vector4.h"

#include <cassert>
#include <array>

namespace CommonUtilities
{
	template <class T>
	class Matrix4x4
	{
	public:
		// Creates the identity matrix.
		Matrix4x4<T>();

		// Copy Constructor.
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);

		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		Matrix4x4<T>& operator=(const Matrix4x4<T> aMatrix);

		// Static functions for creating rotation matrices.
		static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians);

		// Static function for creating a transpose of a matrix.
		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);

		// Assumes aTransform is made of nothing but rotations and translations.
		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);

		const static int mySize = 4;
		std::array<std::array<T, mySize>, mySize> myMatrix;

	private:
	};

	template<class T>
	inline Matrix4x4<T>::Matrix4x4()
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
	inline Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix)
	{
		myMatrix = aMatrix.myMatrix;
	}

	// () operator for accessing element (row, column) for read/write or read, respectively.
	template<class T>
	inline T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		assert(aRow > 0 && "Row index is negative");
		assert(aColumn > 0 && "Column index is negative");
		assert(aRow <= mySize && "Row index is out of bounds");
		assert(aColumn <= mySize && "Column index is out of bounds");

		return myMatrix[aRow - 1][aColumn - 1];
	}

	template<class T>
	inline const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		assert(aRow > 0 && "Row index is negative");
		assert(aColumn > 0 && "Column index is negative");
		assert(aRow <= mySize && "Row index is out of bounds");
		assert(aColumn <= mySize && "Column index is out of bounds");

		return myMatrix[aRow - 1][aColumn - 1];
	}

	template<class T>
	inline const Matrix4x4<T> operator+(const Matrix4x4<T>& aFirstMatrix, const Matrix4x4<T>& aSecondMatrix)
	{
		Matrix4x4<T> tempMatrix;

		for(size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for(size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				tempMatrix(j, i) = aFirstMatrix(j, i) + aSecondMatrix(j, i);
			}
		}

		return tempMatrix;
	}

	template<class T>
	inline void operator+=(Matrix4x4<T>& aFirstMatrix, const Matrix4x4<T> aSecondMatrix)
	{
		for(size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for(size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				aFirstMatrix(j, i) = aFirstMatrix(j, i) + aSecondMatrix(j, i);
			}
		}
	}

	template<class T>
	inline const Matrix4x4<T> operator-(Matrix4x4<T>& aFirstMatrix, const Matrix4x4<T>& aSecondMatrx)
	{
		Matrix4x4<T> tempMatrix;

		for(size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for(size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				tempMatrix(i, j) = aFirstMatrix(i, j) - aSecondMatrx(i, j);
			}
		}

		return tempMatrix;
	}

	template<class T>
	inline void operator-=(Matrix4x4<T>& aFirstMatrix, const Matrix4x4<T> aSecondMatrix)
	{
		Matrix4x4<T> tempMatrix;
		for(size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for(size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				tempMatrix(i, j) = aFirstMatrix(i, j) - aSecondMatrix(i, j);
			}
		}
		aFirstMatrix = tempMatrix;
	}

	template<class T>
	inline const Matrix4x4<T> operator*(const Matrix4x4<T>& aFirstMatrix, const Matrix4x4<T> aSecondMatrix)
	{
		Matrix4x4<T> tempMatrix;

		for(size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for(size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				T a = aFirstMatrix(i, 1);
				T b = aFirstMatrix(i, 2);
				T c = aFirstMatrix(i, 3);
				T d = aFirstMatrix(i, 4);
				T n = aSecondMatrix(1, j);
				T q = aSecondMatrix(2, j);
				T t = aSecondMatrix(3, j);
				T u = aSecondMatrix(4, j);

				tempMatrix(i, j) = a * n + b * q + c * t + d * u;
			}
		}

		return tempMatrix;
	}

	template<class T>
	inline void operator*=(Matrix4x4<T>& aFirstMatrix, const Matrix4x4<T> aSecondMatrix)
	{
		Matrix4x4<T> tempMatrix;

		for(size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for(size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				T a = aFirstMatrix(i, 1);
				T b = aFirstMatrix(i, 2);
				T c = aFirstMatrix(i, 3);
				T d = aFirstMatrix(i, 4);
				T n = aSecondMatrix(1, j);
				T q = aSecondMatrix(2, j);
				T t = aSecondMatrix(3, j);
				T u = aSecondMatrix(4, j);

				tempMatrix(i, j) = a * n + b * q + c * t + d*u;
			}
		}
		aFirstMatrix = tempMatrix;
	}

	template<class T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator=(const Matrix4x4<T> aMatrix)
	{
		for(size_t i = 0; i < mySize; i++)
		{
			for(size_t j = 0; j < mySize; j++)
			{
				myMatrix[j][i] = aMatrix(j + 1, i + 1);
			}
		}
		return *this;
	}


	template<class T>
	inline const bool operator==(const Matrix4x4<T> aFirstMatrix, const Matrix4x4<T> aSecondMatrix)
	{
		for(size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for(size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				if(aFirstMatrix(j, i) != aSecondMatrix(j, i))
				{
					return false;
				}
			}
		}
		return true;
	}

	template<class T>
	inline const bool operator!=(const Matrix4x4<T> aFirstMatrix, const Matrix4x4<T> aSecondMatrix)
	{
		bool isEqual = true;

		for(size_t i = 1; i < aFirstMatrix.mySize + 1; i++)
		{
			for(size_t j = 1; j < aFirstMatrix.mySize + 1; j++)
			{
				if(aFirstMatrix(j, i) != aSecondMatrix(j, i))
				{
					isEqual = false;
				}
			}
		}
		return isEqual;
	}

	template<class T>
	inline const Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		Vector4<T> tempVec;

		tempVec.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1) + aVector.w * aMatrix(4, 1);
		tempVec.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2) + aVector.w * aMatrix(4, 2);
		tempVec.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3) + aVector.w * aMatrix(4, 3);
		tempVec.w = aVector.x * aMatrix(1, 4) + aVector.y * aMatrix(2, 4) + aVector.z * aMatrix(3, 4) + aVector.w * aMatrix(4, 4);

		return tempVec;
	}

	template<class T>
	inline const Vector4<T> operator*(const Matrix4x4<T>& aMatrix, const Vector4<T>& aVector)
	{
		Vector4<T> tempVec;

		tempVec.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1) + aVector.w * aMatrix(4, 1);
		tempVec.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2) + aVector.w * aMatrix(4, 2);
		tempVec.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3) + aVector.w * aMatrix(4, 3);
		tempVec.w = aVector.x * aMatrix(1, 4) + aVector.y * aMatrix(2, 4) + aVector.z * aMatrix(3, 4) + aVector.w * aMatrix(4, 4);

		return tempVec;
	}

	// Static functions for creating rotation matrices.
	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix4x4<T> tempMatrix;

		tempMatrix(1, 1) = 1;
		tempMatrix(2, 2) = cos(aAngleInRadians);
		tempMatrix(2, 3) = sin(aAngleInRadians);
		tempMatrix(3, 2) = -sin(aAngleInRadians);
		tempMatrix(3, 3) = cos(aAngleInRadians);

		return tempMatrix;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix4x4<T> tempMatrix;

		tempMatrix(2, 2) = 1;
		tempMatrix(1, 1) = cos(aAngleInRadians);
		tempMatrix(1, 3) = -sin(aAngleInRadians);
		tempMatrix(3, 1) = sin(aAngleInRadians);
		tempMatrix(3, 3) = cos(aAngleInRadians);

		return tempMatrix;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix4x4<T> tempMatrix;

		tempMatrix(3, 3) = 1;
		tempMatrix(1, 1) = cos(aAngleInRadians);
		tempMatrix(1, 2) = sin(aAngleInRadians);
		tempMatrix(2, 1) = -sin(aAngleInRadians);
		tempMatrix(2, 2) = cos(aAngleInRadians);

		return tempMatrix;
	}

	// Static function for creating a transpose of a matrix.
	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		Matrix4x4<T> tempMatrix;

		for(size_t i = 1; i < mySize + 1; i++)
		{
			for(size_t j = 1; j < mySize + 1; j++)
			{
				tempMatrix(j, i) = aMatrixToTranspose(i, j);
			}
		}

		return tempMatrix;
	}

	// Assumes aTransform is made of nothing but rotations and translations.
	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
	{
		Matrix4x4<T> inversedRotationMatrix = aTransform;
		Matrix4x4<T> inversedTranslationMatrix;
		Matrix4x4<T> resultMatrix;

		// Transpose
		for(size_t row = 1; row < inversedRotationMatrix.mySize; row++)
		{
			for(size_t column = 1; column < inversedRotationMatrix.mySize; column++)
			{
				if(row == column) { continue; }

				inversedRotationMatrix(row, column) = aTransform(column, row);
			}
		}


		// Translate
		resultMatrix = inversedRotationMatrix;

		Vector4<T> translationVector = { -aTransform(4, 1), -aTransform(4, 2), -aTransform(4, 3), 0 };

		translationVector = translationVector * inversedRotationMatrix;

		resultMatrix(4, 1) = translationVector.x;
		resultMatrix(4, 2) = translationVector.y;
		resultMatrix(4, 3) = translationVector.z;

		return resultMatrix; 
	}
}