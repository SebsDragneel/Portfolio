#pragma once
#include "DynArray.h"
template<typename Type>
class BinaryHeap : protected DynArray<Type>
{
public:
	BinaryHeap() {

	}
	~BinaryHeap()
	{
		clear();
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function : enqueue
	// Parameters : v - the item to add to the heap
	// Notes : after the new item is added, this function ensures that the 
	//	smallest value in the heap is in [0]
	/////////////////////////////////////////////////////////////////////////////
	void enqueue(const Type &v)
	{
		DynArray::append(v);
		int I = Size - 1;

		while(true)
		{
			if (array[I] < array[(I - 1) / 2])
			{
				int Temp;
				Temp = array[(I - 1) / 2];
				array[(I - 1) / 2] = array[I];
				array[I] = Temp;

				I = (I - 1) / 2;
			}
			else
				break;
			
		}

	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : dequeue 
		// Return : the smallest item in the heap, or Type() if the heap is empty
		// Notes : after the smallest item is dequeued, this function ensures that 
		//	the smallest item is in [0]
		/////////////////////////////////////////////////////////////////////////////
	Type dequeue()
	{
		if(Size == 0)
		return Type();

		Type Temp = array[0];
		array[0] = array[Size - 1];

		int I = 0;
		while (true)
		{
			if (I * 2 + 1 > Size - 1 || I * 2 + 2 > Size - 1)
				break;
			if (array[I] > array[I * 2 + 1] || array[I] > array[I * 2 + 2])
			{
				Type Temp2 = array[I];

				if (array[I * 2 + 1] <= array[I * 2 + 2])
				{
					array[I] = array[I * 2 + 1];
					array[I * 2 + 1] = Temp2;

					I = I * 2 + 1;
				}
				else if(array[I * 2 + 1] >= array[I * 2 + 2])
				{
					array[I] = array[I * 2 + 2];
					array[I * 2 + 2] = Temp2;
					I = I * 2 + 2;
				}
				else break;
			}
			else break;
		}
		Size--;
		return Temp;

	}

		///////////////////////////////////////////////////////////////////////////////
		// Function : operator[]
		// Parameters : index - the index to access
		// Return : Type& - the item in the index
		/////////////////////////////////////////////////////////////////////////////
	Type& operator[](int index)
	{
		return DynArray::operator[](index);
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : clear	
		// Notes : clears the heap out
		/////////////////////////////////////////////////////////////////////////////
	void clear()
	{
		DynArray::clear();
	}

		///////////////////////////////////////////////////////////////////////////////
		// Function : size
		// Return : the number of valid items in the heap
		/////////////////////////////////////////////////////////////////////////////
	unsigned int size()
	{
		return DynArray::size();
	}


};

