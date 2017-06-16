#pragma once

using namespace std;

template<typename Type>
class BST
{
private:
	struct node
	{
		Type mData;
		typename node* mLeft;
		typename node* mRight;

	};
	unsigned int mSize;
	typename node* mRoot;

	void PreOrder(const node* n)
	{
		if (n != nullptr)
		{
			insert(n->mData);
			PreOrder(n->mLeft);
			PreOrder(n->mRight);
		}
	}
	void PostOrder(const node* n)
	{
		if (n != nullptr)
		{
			PostOrder(n->mLeft);
			PostOrder(n->mRight);
			delete n;

		}
	}

	void InOrder(const node* n) const
	{
		if (n != nullptr)
		{
			InOrder(n->mLeft);
			cout << n->mData << " ";
			InOrder(n->mRight);
		}
	}

public:
	/////////////////////////////////////////////////////////////////////////////
	// Function : Constuctor
	// Notes : constucts an empty BST
	/////////////////////////////////////////////////////////////////////////////
	BST()
	{
		mRoot = nullptr;
		mSize = 0;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function : Destructor
	// Notes : destroys the BST cleaning up any dynamic memory
	/////////////////////////////////////////////////////////////////////////////
	~BST() { clear(); }

	/////////////////////////////////////////////////////////////////////////////
	// Function : assignment operator
	/////////////////////////////////////////////////////////////////////////////
	BST& operator=(const BST& that)
	{
		node* mTemp = that.mRoot;

		if (this != &that)
		{
			this->clear();
			PreOrder(mTemp);
		}
		return *this;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function: copy constructor
	/////////////////////////////////////////////////////////////////////////////
	BST(const BST& that)
	{
		mRoot = nullptr;
		*this = that;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function : insert
	// Parameters :  v - the item to insert 
	/////////////////////////////////////////////////////////////////////////////
	void insert(const Type& v)
	{
		node* n = new node();
		n->mData = v;
		n->mLeft = nullptr;
		n->mRight = nullptr;

		if (mRoot == nullptr)
		{
			mRoot = n;
			++mSize;
			return;
		}

		node* mCurrent = mRoot;

		while (true)
		{
			if (n->mData < mCurrent->mData)
			{
				if (mCurrent->mLeft == nullptr)
				{
					mCurrent->mLeft = n;
					++mSize;
					return;
				}
				else
				{
					mCurrent = mCurrent->mLeft;

				}
			}
			else if (n->mData >= mCurrent->mData)
			{
				if (mCurrent->mRight == nullptr)
				{
					mCurrent->mRight = n;
					++mSize;
					return;
				}
				else
				{
					mCurrent = mCurrent->mRight;
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function : findAndRemove
	// Parameters : v - the item to find (and remove if it is found)
	// Return : bool - true if the item was removed, false otherwise
	/////////////////////////////////////////////////////////////////////////////
	bool findAndRemove(const Type& v)
	{
		node* mParent = mRoot;
		node* mCurrent = mRoot;
		while (true)
		{
			if (mCurrent == nullptr)
			{
				return false;
			}
			else if (mCurrent->mData > v)
			{
				mParent = mCurrent;
				mCurrent = mCurrent->mLeft;
			}
			else if (mCurrent->mData < v)
			{
				mParent = mCurrent;
				mCurrent = mCurrent->mRight;
			}
			else
			{
				if (mCurrent->mLeft == nullptr && mCurrent->mRight == nullptr)
				{
					if (mCurrent == mRoot)
					{
						delete mCurrent;
						mRoot = nullptr;
						--mSize;
						return true;
					}
					else if (mParent->mRight == mCurrent)
					{
						delete mCurrent;
						mParent->mRight = nullptr;
						--mSize;
						return true;
					}
					else if (mParent->mLeft == mCurrent)
					{
						delete mCurrent;
						mParent->mLeft = nullptr;
						--mSize;
						return true;
					}
				}
				else if ((mCurrent->mLeft != nullptr && mCurrent->mRight == nullptr) || (mCurrent->mRight != nullptr && mCurrent->mLeft == nullptr))
				{
					if (mCurrent == mRoot && mCurrent->mRight == nullptr)
					{
						mRoot = mCurrent->mLeft;
						delete mCurrent;
						--mSize;
						return true;
					}
					else if (mCurrent == mRoot && mCurrent->mLeft == nullptr)
					{
						mRoot = mCurrent->mRight;
						delete mCurrent;
						--mSize;
						return true;
					}
					else if (mCurrent == mParent->mLeft)
					{
						if (mCurrent->mRight == nullptr)
						{
							mParent->mLeft = mCurrent->mLeft;
							delete mCurrent;
							--mSize;
							return true;
						}
						else if (mCurrent->mLeft == nullptr)
						{
							mParent->mLeft = mCurrent->mRight;
							delete mCurrent;
							--mSize;
							return true;
						}
					}
					else if (mCurrent == mParent->mRight)
					{
						if (mCurrent != mRoot && mCurrent->mRight == nullptr)
						{
							mParent->mRight = mCurrent->mLeft;
							delete mCurrent;
							--mSize;
							return true;
						}
						else if (mCurrent != mRoot && mCurrent->mLeft == nullptr)
						{
							mParent->mRight = mCurrent->mRight;
							delete mCurrent;
							--mSize;
							return true;
						}
					}
				}
				else if ((mCurrent->mLeft != nullptr && mCurrent->mRight != nullptr))
				{
					node* mMin = mRoot;
					mMin = mCurrent->mRight;
					while (mMin->mLeft != nullptr)
					{
						mParent = mMin;
						mMin = mMin->mLeft;

					}
					Type mTemp = mCurrent->mData;
					mCurrent->mData = mMin->mData;
					mMin->mData = mTemp;
					mCurrent = mMin;
					continue;

				}

			}
		}

	}
	/////////////////////////////////////////////////////////////////////////////
	// Function : find
	// Parameters : v - the item to find
	// Return : bool - true if the item was found, false otherwise
	/////////////////////////////////////////////////////////////////////////////
	bool find(const Type& v) const
	{
		node* mCurrent = mRoot;

		while (true)
		{
			if (mCurrent == nullptr)
			{
				return false;
			}
			else if (mCurrent->mData > v)
			{
				mCurrent = mCurrent->mLeft;
			}
			else if (mCurrent->mData < v)
			{
				mCurrent = mCurrent->mRight;
			}
			else
			{
				return true;
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	// Function : clear
	// Notes : clears the BST, cleaning up any dynamic memory
	/////////////////////////////////////////////////////////////////////////////
	void clear()
	{
		PostOrder(mRoot);
		mRoot = nullptr;
		mSize = 0;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function : printInOrder
	// Notes : prints the contents of the BST to the screen, in ascending order
	/////////////////////////////////////////////////////////////////////////////
	void printInOrder() const
	{
		InOrder(mRoot);
		cout << "\n";

	}



};

