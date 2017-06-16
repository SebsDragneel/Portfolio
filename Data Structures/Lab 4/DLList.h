#pragma once
template<typename Type> class DLLIter;

template<typename Type>
class DLList
{
private:
	friend class DLLIter<Type>;

	struct node
	{
		Type mData;
		typename DLList<Type>::node* mPrev;
		typename DLList<Type>::node* mNext;

	};
	unsigned int mSize;
	typename DLList<Type>::node* mHead;
	typename DLList<Type>::node* mTail;

public:
	/////////////////////////////////////////////////////////////////////////////
	// Function : Constructor
	// Notes : constructs an empty list
	/////////////////////////////////////////////////////////////////////////////
	DLList()
	{
		mHead = nullptr;
		mTail = nullptr;
		mSize = 0;
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : Destructor
		// Notes : Destroys a list
		/////////////////////////////////////////////////////////////////////////////
	~DLList()
	{
		clear();
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : Assignment Operator
		/////////////////////////////////////////////////////////////////////////////
	DLList<Type>& operator=(const DLList<Type>& that)
	{
		

		if (this != &that)
		{
			clear();
			node* mTemp = that.mHead;
			for (unsigned int i = 0; i < that.mSize; i++)
			{
				Type& TempData = mTemp->mData;
				if (i == 0)
				{
					addHead(TempData);
				}
				else
				{
					addTail(TempData);
				}

				mTemp = mTemp->mNext;
			}
		}

		return *this;
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : Copy Constructor
		/////////////////////////////////////////////////////////////////////////////
	DLList(const DLList<Type>& that)
	{
		mHead = nullptr;
		mTail = nullptr;
		*this = that;
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : addHead
		// Parameters : v - the item to add to the head of the list
		/////////////////////////////////////////////////////////////////////////////
	void addHead(const Type& v)
	{
		if (mHead == nullptr)
		{
			node* mTemp = new node;
			mTemp->mData = v;
			mHead = mTail = mTemp;
			mHead->mNext = nullptr;
			mTail->mPrev = nullptr;
		}
		else
		{
			node* mTemp = new node;
			mTemp->mData = v;
			mHead->mPrev = mTemp;
			mTemp->mNext = mHead;
			mHead = mTemp;
			mHead->mPrev = nullptr;
		}
		++mSize;
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : addTail
		// Parameters : v - the item to add to the tail of the list
		/////////////////////////////////////////////////////////////////////////////
	void addTail(const Type& v)
	{
		if (mTail == nullptr)
		{
			node* mTemp = new node;
			mTemp->mData = v;
			mHead = mTail = mTemp;
			mHead->mNext = nullptr;
			mTail->mPrev = nullptr;
		}
		else
		{
			node* mTemp = new node;
			mTemp->mData = v;
			mTail->mNext = mTemp;
			mTemp->mPrev = mTail;
			mTail = mTemp;
			mTail->mNext = nullptr;
		}
		++mSize;
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : clear
		// Notes : clears the list, freeing any dynamic memory
		/////////////////////////////////////////////////////////////////////////////
	void clear()
	{
		node* mTemp = mHead;

		while (mTemp != nullptr)
		{
			mTemp = mHead->mNext;
			delete mHead;
			mHead = mTemp;
		}
		mHead = nullptr;
		mTail = nullptr;
		mSize = 0;

	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : insert
		// Parameters :	index - an iterator to the location to insert at
		//				v - the item to insert
		// Notes : do nothing on a bad iterator
		/////////////////////////////////////////////////////////////////////////////
	void insert(DLLIter<Type>& index, const Type& v)
	{
		if (index.mCurrent == nullptr)
		{
			return;
		}
			if (index.mCurrent == mHead)
			{
				addHead(v);
				index.beginHead();
				return;
			}
			else
			{
				node* mTemp = new node;
				mTemp->mPrev = index.mCurrent->mPrev;
				mTemp->mNext = index.mCurrent;
				index.mCurrent->mPrev->mNext = mTemp;
				index.mCurrent->mPrev = mTemp;
				mTemp->mData = v;
				index.mCurrent = mTemp;
				++mSize;
				return;
			}
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : remove
		// Parameters :	index - an iterator to the location to remove from
		// Notes : do nothing on a bad iterator
		/////////////////////////////////////////////////////////////////////////////
	void remove(DLLIter<Type>& index)
	{
		if (index.mCurrent == nullptr)
			return;
		
		//index.beginHead();
		
		if (mSize == 0)
		{
			mHead = nullptr;
			mTail = nullptr;
			index.mCurrent = nullptr;
			return;
		}

		

			if (index.mCurrent == mHead && mHead != nullptr)
			{
				node* mTemp = mHead;
				index.mCurrent = index.mCurrent->mNext;
				mHead = index.mCurrent;

				delete mTemp;
				index.mCurrent = mHead;
				--mSize;

				return;
			}
			else if (index.mCurrent == mTail && mHead != nullptr)
			{
				node* mTemp = mTail;
				mTail = index.mCurrent->mPrev;
				delete mTemp;
				index.mCurrent = mTail;
				--mSize;
				return;
			}
			else
			{
				index.mCurrent->mPrev->mNext = index.mCurrent->mNext;
				node* mTemp = index.mCurrent;
				index.mCurrent = index.mCurrent->mNext;
				delete mTemp;
				--mSize;
				return;
			}
	}

};

template<typename Type>
class DLLIter
{
private:
	friend class DLList<Type>;
	DLList<Type>* List;
	typename DLList<Type>::node* mCurrent;

public:
	/////////////////////////////////////////////////////////////////////////////
	// Function : Constructor
	// Parameters :	listToIterate - the list to iterate
	/////////////////////////////////////////////////////////////////////////////
	DLLIter(DLList<Type>& listToIterate)
	{
		List = &listToIterate;
		beginHead();
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : beginHead
		// Notes : moves the iterator to the head of the list
		/////////////////////////////////////////////////////////////////////////////
	void beginHead()
	{
		mCurrent = List->mHead;
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : beginTail
		// Notes : moves the iterator to the tail of the list
		/////////////////////////////////////////////////////////////////////////////
	void beginTail()
	{
		mCurrent = List->mTail;
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : end
		// Notes : returns true if we are at the end of the list, false otherwise
		/////////////////////////////////////////////////////////////////////////////
	bool end() const
	{
		if (mCurrent == nullptr)
			return true;
		else return false;

	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : operator++
		// Notes : move the iterator forward one node
		/////////////////////////////////////////////////////////////////////////////
	DLLIter<Type>& operator++()
	{
		if (mCurrent == nullptr)
		{
			return *this;
		}
		else
		{
			mCurrent = mCurrent->mNext;
			return *this;
		}
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : operator--
		// Notes : move the iterator backward one node
		/////////////////////////////////////////////////////////////////////////////
	DLLIter<Type>& operator--()
	{
		if (mCurrent == nullptr)
		{
			return *this;
		}
		else
		{
			mCurrent = mCurrent->mPrev;
			return *this;
		}
	}


		/////////////////////////////////////////////////////////////////////////////
		// Function : current
		// Notes : returns the item at the current iterator location
		////////////////////////////////////////////////////////////////////////////
	Type& current() const
	{
		if(mCurrent != nullptr)
		return mCurrent->mData;
	}

};
