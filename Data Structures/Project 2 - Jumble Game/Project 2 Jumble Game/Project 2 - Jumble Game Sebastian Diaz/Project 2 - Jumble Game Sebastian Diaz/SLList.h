#pragma once
template<typename Type> class SLLIter;

template<typename Type>
class SLList
{
private:
	friend class SLLIter<Type>;

	struct node
	{
		Type mData;
		typename SLList<Type>::node* mNext;

	};
	unsigned int mSize;
	typename SLList<Type>::node* mHead;

public:
	/////////////////////////////////////////////////////////////////////////////
	// Function : Constructor
	// Notes : constructs an empty list
	/////////////////////////////////////////////////////////////////////////////
	SLList()
	{
		mHead = nullptr;
		mSize = 0;
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : Destructor
		// Notes : Destroys the list
		/////////////////////////////////////////////////////////////////////////////
	~SLList()
	{
		clear();
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : Assignment Operator
		/////////////////////////////////////////////////////////////////////////////
	SLList<Type>& operator=(const SLList<Type>& that)
	{
		node* mTemp = that.mHead;

		if (this != &that)
		{
			this->clear();
			SLLIter<Type> ThisIter(*this);
			ThisIter.begin();
			for (unsigned int i = 0; i < that.size(); ++i)
			{
					Type& TempData = mTemp->mData;
					node* NewNode = new node();
					NewNode->mData = TempData;
				if (i == 0)
				{
					addHead(TempData);
				}
				else
				{
					this->insert(ThisIter, TempData);

				}
				
				
				mTemp = mTemp->mNext;
				++ThisIter;

			}
		}
		
		return *this;
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : Copy Constructor
		/////////////////////////////////////////////////////////////////////////////
	SLList(const SLList<Type>& that)
	{
		mHead = nullptr;
		*this = that;
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : addHead
		// Parameters :	v - the item to add to the list
		/////////////////////////////////////////////////////////////////////////////
	void addHead(const Type& v)
	{
		if (mHead)
		{
			node* mTemp = new node;
			mTemp->mData = v;
			mTemp->mNext = mHead;
			mHead = mTemp;
		}
		else
		{
			mHead = new node;
			mHead->mData = v;
			mHead->mNext = nullptr;
		}

		++mSize;

	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : clear
		// Notes : clears the list, freeing any dynamic memory
		/////////////////////////////////////////////////////////////////////////////
	void clear()
	{
		node* mTemp;

		while (mHead != nullptr)
		{
			mTemp = mHead;
			mHead = mHead->mNext;
			delete mTemp;
		}
		mSize = 0;
		mHead = nullptr;
	}
	/////////////////////////////////////////////////////////////////////////////
	// Function : insert
	// Parameters :	index - an iterator to the location to insert at
	//				v - the item to insert
	// Notes : do nothing on a bad iterator
	/////////////////////////////////////////////////////////////////////////////
	void insert(SLLIter<Type>& index, const Type& v)
	{
		
		node* Current = index.List->mHead;
		if (Current == nullptr)
		{
		return;
		}
		while (true)
		{
			if (index.mCurrent == mHead)
			{
				addHead(v);
				index.begin();
				++mSize;
				break;
			}
			else if (index.mCurrent == Current)
			{
				node* mTemp = new node;
				index.mPrev->mNext = mTemp;
				mTemp->mNext = Current;
				mTemp->mData = v;
				Current = mTemp;
				index.mCurrent = Current;
				++mSize;
				break;
			}
			index.mPrev = Current;
			Current = Current->mNext;
		}

	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : remove
		// Parameters :	index - an iterator to the location to remove from
		// Notes : do nothing on a bad iterator
		/////////////////////////////////////////////////////////////////////////////
		void remove(SLLIter<Type>& index)
	{
		if (index.mCurrent == nullptr)
			return;
		Type DatatoDelete = index.current();
		index.begin();
		
		
		while (true)
		{
			
			 if (DatatoDelete == mHead->mData)
			{
				index.mPrev = index.mCurrent;
				++index;
				delete index.mPrev;
				mHead = index.mCurrent;
				--mSize;
				break;
			}
			else if (index.current() == DatatoDelete)
			{
				index.mPrev->mNext = index.mCurrent->mNext;
				node* mTemp = index.mCurrent;
				index.mCurrent = index.mCurrent->mNext;
				delete mTemp;
				--mSize;
				break;
			}
			index.mPrev = index.mCurrent;
			index.mCurrent = index.mCurrent->mNext;
		}

	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : size
		// Return : the number of items stored in the linked list.
		/////////////////////////////////////////////////////////////////////////////
		inline unsigned int size() const { return mSize; }

};

template<typename Type>
class SLLIter
{
private:
	friend class SLList<Type>;
	SLList<Type>* List;
	typename SLList<Type>::node* mCurrent;
	typename SLList<Type>::node* mPrev;

public:
	/////////////////////////////////////////////////////////////////////////////
	// Function : Constructor
	// Parameters :	listToIterate - the list to iterate
	/////////////////////////////////////////////////////////////////////////////
	SLLIter(SLList<Type>& listToIterate)
	{
		List = &listToIterate;
		begin();
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : begin
		// Notes : moves the iterator to the head of the list
		/////////////////////////////////////////////////////////////////////////////
		void begin() {
			mPrev = nullptr;
			mCurrent = List->mHead;

	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : end
		// Notes : returns true if we are at the end of the list, false otherwise
		/////////////////////////////////////////////////////////////////////////////
	bool end() const {
		if (mCurrent == nullptr)
			return true;
		else return false;

	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : operator++
		// Notes : move the iterator forward one node
		/////////////////////////////////////////////////////////////////////////////
	SLLIter<Type>& operator++()
	{
		if (mCurrent == nullptr)
		{
			return *this;
		}
		else
		{
			mPrev = mCurrent;
			mCurrent = mCurrent->mNext;
			return *this;
		}
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : current
		// Notes : returns the item at the current iterator location
		/////////////////////////////////////////////////////////////////////////////
	Type& current() const {

		return mCurrent->mData;
	}

};