#pragma once
#include <iostream>

template<class ArrayDataType>
class DynamicArray
{
private:
	typedef ArrayDataType* iterator;
	typedef const ArrayDataType* const_iterator;

public:
	int ArraySize;			// Array starting size and the size after it is resized
	ArrayDataType* Array;	// Pointer to Array
	int ArrayIndexPointer;	// keeps track amount of objects in array
	int ArrayStepSize;		// how much the array will get increased by when it reaches the limit

#pragma region Constructors
	/// <summary>
	/// Constructor to set all the variables
	/// </summary>
	DynamicArray()																// Constructor
	{
	// Default values, can be changed
		ArraySize = 10;
		ArrayStepSize = 5;

		// ! DO NOT CHANGE !
		ArrayIndexPointer = 0;
		Array = new ArrayDataType[ArraySize]();
	}

	/// <summary>
	/// Constructor with starting size and step size params for custom objects
	/// </summary>
	/// <param name="StartSize"> - Starting size of the array</param>
	/// <param name="StepSize"> - how much the array will increase each time it reaches the limit</param>
	DynamicArray(int StartSize, int StepSize)									// Constructor with starting size and step size params
	{
		ArraySize = StartSize;
		ArrayStepSize = StepSize;

		// ! DO NOT CHANGE !
		ArrayIndexPointer = 0;
		Array = new ArrayDataType[ArraySize]();
	}
#pragma endregion

#pragma region Array Modification
	/// <summary>
	/// Appends another array to the DynamicArray
	/// </summary>
	/// <param name="ArrayToAdd"> - the array to append</param>
	/// <param name="size"> - size of the array</param>
	/// <param name="includeEscape">(default = false) -  !TEXT DATA TYPES ONLY! if escape characters should be appended as well</param>
	void ArrayAppend(ArrayDataType ArrayToAdd[], int size, bool includeEscape = false)	// Adds an array to the Dynamic Array
	{
		if (std::is_same<ArrayDataType, std::string>::value || std::is_same<ArrayDataType, char>::value) // For Text types only
		{
			for (int i = 0; i < size; ++i) // for loop to get and append all character
			{
				if (includeEscape) // if IncludeEscape is true, append all character
				{
					Append(ArrayToAdd[i]); // Append character to array
				}
				else if (!(ArrayToAdd[i] == 0)) // if IncludeEscape is false, include all chars that aren't null
				{
					Append(ArrayToAdd[i]); // Append character to array
				}
			}
		}
		else
		{
			for (int i = 0; i < size; ++i) // for loop to get and append all Array Objects
			{
				Append(ArrayToAdd[i]);
			}
		}
	}

	/// <summary>
	/// Append single Object
	/// </summary>
	/// <param name="ObjectToAdd"> - Object to add</param>
	void Append(ArrayDataType ObjectToAdd)												// Adds a Object to the Array
	{
		if (ArrayIndexPointer >= ArraySize) // if Current Index pointer is more then the array size (trying to add to OutOfRange space)
		{
			ArrayDataType* TempArray = new ArrayDataType[ArraySize](); // Create new array which will store the original values

			for (int i = 0; i < ArraySize; i++) // assign/copy all values from Array to Temp
			{
				TempArray[i] = Array[i];
			}

			ArraySize += ArrayStepSize; // expand the Array size
			Array = new ArrayDataType[ArraySize](); // over ride Array with new, bigger, array

			/*
			ArraySize-ArrayStepSize calculates TempArray size
			Copy all values from Temp array to "old" expanded array
			*/
			for (int i = 0; i < ArraySize - ArrayStepSize; i++)
			{
				Array[i] = TempArray[i];

			}

			delete[] TempArray;
		}

		Array[ArrayIndexPointer] = ObjectToAdd;
		ArrayIndexPointer++;
	}

	/// <summary>
	/// Replaces a a chosen positin with the chosen Object
	/// </summary>
	/// <param name="ReplaceObject"> - Object to place in the position</param>
	/// <param name="position"> - position to put the Object in</param>
	void Replace(ArrayDataType ReplaceObject, int position)								// Replaces a Object in the Array
	{
		if (position >= (ArrayIndexPointer - 1) || position < 0)// check if the position to remove is in array range
		{
			throw std::out_of_range("position was out of range of the array");
			return;
		}

		Array[position] = ReplaceObject;
	}

	/// <summary>
	/// Remove and move all Object infront, back 1 spot
	/// </summary>
	/// <param name="position"> - Position to remove</param>
	void Remove(int position)															// Removes Object and moves everything infront of it back
	{
		if (position >= (ArrayIndexPointer - 1) || position < 0)// check if the position to remove is in array range
		{
			throw std::out_of_range("position was out of range of the array");
			return;
		}

		for (int i = position; i < (ArrayIndexPointer - 1); i++) // moving all back
		{
			Array[i] = Array[i + 1];
		}
		Array[ArrayIndexPointer - 1] = NULL; // make last character blank
		ArrayIndexPointer--;
	}

	/// <summary>
	/// Clear the dynamic array to the original size
	/// </summary>
	void Clear()																		// basically restart the array
	{
		ArrayIndexPointer = 0;
		Array = new ArrayDataType[ArraySize]();
	}
#pragma endregion

#pragma region Variables
	// Get private variables
	ArrayDataType* GetArray()
	{
		return Array;
	}

	int GetArraySize()
	{
		return ArraySize;
	}

	int GetArrayIndexPointer()
	{
		return ArrayIndexPointer;
	}

	int GetArrayStepSize()
	{
		return ArrayStepSize;
	}
#pragma endregion

#pragma region For Loop Functions
	// For loop range-based function
	iterator begin() { return &Array[0]; }
	const_iterator begin() const { return &Array[0]; }
	iterator end() { return &Array[ArrayIndexPointer]; }
	const_iterator end() const { return &Array[ArrayIndexPointer]; }
#pragma endregion

#pragma region Operators
	friend std::ostream& operator<<(std::ostream& os, const DynamicArray& Array)
	{
		if (std::is_same<ArrayDataType, std::string>::value || std::is_same<ArrayDataType, char>::value) // if type is text type, output as text
		{
			os << Array.Array;
		}
		else  // if type is any other "list" the array with comman
		{
			for (int i = 0; i < Array.ArrayIndexPointer; i++)
			{
				os << Array.Array[i];

				if (!(i == Array.ArrayIndexPointer - 1))
				{
					os << ", ";
				}
			}
		}
		return os;
	}

	/// <summary>
	/// [] operator which acts the same as the Array [] operator
	/// </summary>
	/// <param name="position"> - position of the value wanted</param>
	/// <returns>value in the position</returns>
	ArrayDataType& operator[](int position)
	{
		return Array[position];
	}
#pragma endregion
};