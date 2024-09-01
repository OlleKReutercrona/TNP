#pragma once
#include <array>
#include <cassert>

#define VECT_CLASS_TYPENAME template <typename Type, int size, typename CountType = unsigned short, bool UseSafeModeFlag = true>
#define VECT_METHOD_TYPENAME template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
namespace CommonUtilities
{

	VECT_CLASS_TYPENAME
    class VectorOnStack
    {
    public:
        VectorOnStack();
        VectorOnStack(const VectorOnStack& aVectorOnStack);
        VectorOnStack(const std::initializer_list<Type>& anInitList);

        ~VectorOnStack();

        VectorOnStack& operator=(const VectorOnStack& aVectorOnStack);
        inline Type& operator[](const CountType anIndex);
        inline const Type& operator[](const CountType anIndex) const;

        inline void Add(const Type& anObject);
        inline void Insert(const CountType anIndex, const Type& anObject);
        inline void RemoveCyclic(const Type& anObject);
        inline void RemoveCyclicAtIndex(const CountType anItemNumber);
        inline void Clear();

        __forceinline CountType Size() const;
    private:

        Type myElements[size];
        int myCurrentMaxIndex = -1;
    };

#pragma region Methods


	VECT_METHOD_TYPENAME
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::VectorOnStack()
	{
		
	}

	VECT_METHOD_TYPENAME
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::VectorOnStack(const VectorOnStack& aVectorOnStack)
	{
		if(UseSafeModeFlag)
		{
			for(size_t i = 0; i < size; i++)
			{
				++myCurrentMaxIndex;
				myElements[i] = aVectorOnStack.myElements[i];
			}
		}
		else
		{
			memcpy(&myElements, &aVectorOnStack.myElements, sizeof(aVectorOnStack.myElements));
			memcpy(&myCurrentMaxIndex, &aVectorOnStack.myCurrentMaxIndex, sizeof(aVectorOnStack.myCurrentMaxIndex));

			return;
		}
	}

	VECT_METHOD_TYPENAME
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::VectorOnStack(const std::initializer_list<Type>& anInitList) // VG
	{
		assert(anInitList.size() <= size && "Initializer list size is larger than vector size.");

		myCurrentMaxIndex = static_cast<int>(anInitList.size()) - 1;
		

		if(UseSafeModeFlag)
		{
			int count = 0;
			for(Type element : anInitList)
			{
				myElements[count] = element;
				count++;
			}
		}
		else
		{
			int count = 0;
			for(Type element : anInitList)
			{
				memcpy(&myElements[count], &element, sizeof(element));
				count++;
			}
		}
	}

	VECT_METHOD_TYPENAME
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::~VectorOnStack()
	{
		myCurrentMaxIndex = -1;
	}

	VECT_METHOD_TYPENAME
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::operator=(const VectorOnStack& aVectorOnStack)
	{
		if(this == &aVectorOnStack)
		{
			return *this;
		}

		if(UseSafeModeFlag)
		{
			for(size_t i = 0; i < size; i++)
			{
				++myCurrentMaxIndex;
				myElements[i] = aVectorOnStack.myElements[i];
			}
		}
		else
		{
			for(size_t i = 0; i < size; i++)
			{
				++myCurrentMaxIndex;
				memcpy(&myElements[i], &aVectorOnStack.myElements[i], sizeof(aVectorOnStack.myElements[i]));
			}

		}
		return *this;
	}

	VECT_METHOD_TYPENAME
	inline Type& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::operator[](const CountType anIndex)
	{
		assert(anIndex < size && "Index is out of bounds.");
		assert(anIndex >= 0 && "Index is negative");
		assert(anIndex <= myCurrentMaxIndex && "Index is out of bounds");

		return myElements[anIndex];
	}

	VECT_METHOD_TYPENAME
	inline const Type& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::operator[](const CountType anIndex) const
	{
		assert(anIndex < size && "Index is out of bounds.");
		assert(anIndex >= 0 && "Index is negative");
		assert(anIndex <= myCurrentMaxIndex && "Index is out of bounds");

		return myElements[anIndex];
	}

	VECT_METHOD_TYPENAME
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Add(const Type& anObject)
	{
		assert((myCurrentMaxIndex + 1) < size);
		++myCurrentMaxIndex;

		if(UseSafeModeFlag)
		{
			myElements[myCurrentMaxIndex] = anObject;
		}
		else
		{
			memcpy(&myElements[myCurrentMaxIndex], &anObject, sizeof(anObject));
		}
	}

	VECT_METHOD_TYPENAME
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Insert(const CountType anIndex, const Type& anObject)
	{
		assert((myCurrentMaxIndex + 1) < size && "Vector is full");
		assert(anIndex >= 0 && "Index is negative");
		assert(anIndex < size && "Index is out of bounds");

		++myCurrentMaxIndex;

		if(UseSafeModeFlag)
		{
			for(int i = myCurrentMaxIndex; i > anIndex; --i)
			{
				myElements[i] = myElements[i - 1];
			}
			myElements[anIndex] = anObject;
		}
		else
		{
			for(int i = myCurrentMaxIndex; i > anIndex; --i)
			{
				memcpy(&myElements[i], &myElements[i - 1], sizeof(myElements[i - 1]));
			}
			memcpy(&myElements[anIndex], &anObject, sizeof(anObject));

			return;
		}
	}

	VECT_METHOD_TYPENAME
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::RemoveCyclic(const Type& anObject)
	{
		for(int i = 0; i < myCurrentMaxIndex + 1; ++i)
		{
			if(myElements[i] == anObject)
			{
				myElements[i] = myElements[myCurrentMaxIndex];
				myCurrentMaxIndex--;
			}
		}
	}

	VECT_METHOD_TYPENAME
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::RemoveCyclicAtIndex(const CountType anIndex)
	{
		assert(anIndex <= myCurrentMaxIndex && "Index is out of bounds");
		assert(anIndex >= 0 && "Index is negative");

		myElements[anIndex] = myElements[myCurrentMaxIndex];
		myCurrentMaxIndex--;

	}

	VECT_METHOD_TYPENAME
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Clear()
	{
		myCurrentMaxIndex = -1;
	}

	VECT_METHOD_TYPENAME
	CountType VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Size() const
	{
		return static_cast<CountType>(myCurrentMaxIndex + 1);
	}

#pragma endregion
}