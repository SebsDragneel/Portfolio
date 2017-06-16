#pragma once
#include "SLList.h"
using namespace std;

template<typename Type>
class HTable
{
private:
	SLList<Type> * mTable; 
	unsigned int mNumofBuckets;
	unsigned int(*HashFunction)(const Type &v);
public:
	/////////////////////////////////////////////////////////////////////////////
	// Function : Constructor 
	// Parameters : numOfBuckets - the number of buckets
	//              hFunction - pointer to the hash function for this table
	// Notes : constructs an empty hash table
	/////////////////////////////////////////////////////////////////////////////
	HTable(unsigned int numOfBuckets, unsigned int(*hFunction) (const Type &v))
	{
		mTable = new SLList<Type>[numOfBuckets]; // remember to delete
		mNumofBuckets = numOfBuckets;
		HashFunction = hFunction;
	
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : Destructor
		// Notes : destroys a hash table
		/////////////////////////////////////////////////////////////////////////////
	~HTable() { delete[] mTable; }

		/////////////////////////////////////////////////////////////////////////////
		// Function : Assignment Operator
		/////////////////////////////////////////////////////////////////////////////
		HTable<Type>& operator=(const HTable<Type>& that)
		{
			if (this != &that)
			{
				clear();
				delete[] mTable;

				mTable = new SLList<Type>[mNumofBuckets]; // remember to delete
				mNumofBuckets = that.mNumofBuckets;
				HashFunction = that.HashFunction;


				for (unsigned int i = 0; i < mNumofBuckets; i++)
				{
					mTable[i] = that.mTable[i];
				}
			}
			return *this;
		}

		/////////////////////////////////////////////////////////////////////////////
		// Function : Copy Constructor
		/////////////////////////////////////////////////////////////////////////////
		HTable(const HTable<Type>& that)
		{
			
			*this = that;

		}
		// Function : insert

		/////////////////////////////////////////////////////////////////////////////
		// Parameters : v - the item to insert into the hash table
		/////////////////////////////////////////////////////////////////////////////
		void insert(const Type& v)
		{	
			mTable[HashFunction(v)].addHead(v);
		}

		/////////////////////////////////////////////////////////////////////////////
		// Function : findAndRemove
		// Parameters : v - the item to remove(if it is found)
		/////////////////////////////////////////////////////////////////////////////
		bool findAndRemove(const Type& v)
		{
			unsigned int mIndex = HashFunction(v);
			SLLIter<Type> Itter(mTable[mIndex]);
			for (Itter.begin(); !Itter.end(); ++Itter)
			{
				if (Itter.current() == v)
				{
					mTable[mIndex].remove(Itter);
					return true;
				}
			}
			return false;
		}



		/////////////////////////////////////////////////////////////////////////////
		// Function : clear
		// Notes : clears the hash table
		/////////////////////////////////////////////////////////////////////////////
		void clear()
		{
			for (unsigned int  i = 0; i < mNumofBuckets; i++)
			{
				mTable[i].clear();
			}
		}

		/////////////////////////////////////////////////////////////////////////////
		// Function : find
		// Parameters : v - the item to look for
		// Return : the bucket we found the item in or -1 if we didn’t find the item.
		/////////////////////////////////////////////////////////////////////////////
		int find(const Type& v) const
		{
			unsigned int mIndex = HashFunction(v);
			SLLIter<Type> Itter = mTable[mIndex];
			for (Itter.begin(); !Itter.end(); ++Itter)
			{
				if (Itter.current() == v)
				{
					return mIndex;
				}
			}
			return -1;

		}

			void HTable<Type>::printSomeStuff(const char* filePath = "hashdata.txt")
		{
			// we're gonna need to open a file for all this data
			ofstream outFile(filePath);

			// let's make sure the file got opened
			if (outFile.is_open())
			{
				// number of empty buckets, total count of elements, index of emptiest bucket, index of fullest bucket
				unsigned int empty = 0;
				unsigned int totalCount = 0;
				unsigned int loIndex = 0;
				unsigned int hiIndex = 0;

				// loop through all the buckets
				for (unsigned int i = 0; i < mNumofBuckets; ++i)
				{
					// add the number of elements in each bucket to the total count
					totalCount += mTable[i].size();
					// print the index of this bucket and its size to the file
					outFile << i << " : " << mTable[i].size() << '\n';

					// if this list is empty, increment the empty count
					if (mTable[i].size() == 0)
						++empty;

					// if this list has less elements than the lowest one recorded so far, store this as the new lowest
					if (mTable[i].size() < mTable[loIndex].size())
						loIndex = i;
					// else, if this list has more elements than the highest one recorded so far, store this as the new highest
					else if (mTable[i].size() > mTable[hiIndex].size())
						hiIndex = i;
				}

				// print the total count of items and number of buckets to the file
				outFile << '\n' << totalCount << " Total items stored in " << mNumofBuckets << " buckets\n";
				// print the number of empty buckets
				outFile << '\n' << empty << " Buckets are empty\n\n";
				// get the number of elements in the emptiest bucket
				unsigned int Low = mTable[loIndex].size();
				// get the range fullest-emptiest+1
				unsigned int range = mTable[hiIndex].size() - Low + 1;
				// print this info to the file
				outFile << "each bucket contains between " << Low << " and " << Low + range - 1 << " items.\n\n";

				// make a new array to count how many buckets contain each number of items between the emptiest and fullest
				// and initialize each cell to 0
				unsigned int* arr = new unsigned int[range];
				for (unsigned int j = 0; j < range; ++j)
					arr[j] = 0;

				// now we go through and count how many buckets contain each number of items...
				// 3 buckets have 15 items
				// 5 buckets have 16 items
				// etc.
				for (unsigned int k = 0; k < mNumofBuckets; ++k)
					++arr[mTable[k].size() - Low];

				// now print this data to the file
				for (unsigned int p = 0; p < range; ++p)
					outFile << arr[p] << " buckets have " << p + Low << " items\n";

				// delete the array we made a minute ago, we are done with it
				delete[] arr;
			}
		}
};
