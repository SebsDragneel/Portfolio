#pragma once
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include "BitStream.h"
using namespace std;



void PostOrder(const HuffNode* n)
{
	if (n != nullptr)
	{
		PostOrder(n->left);
		PostOrder(n->right);
		delete n;

	}
}
struct cmp
{
	bool operator() (const HuffNode * a, const HuffNode * b)
	{
		return (a->freq > b->freq);
	}
};

	/////////////////////////////////////////////////////////////////////////////
	// Function : generateFrequencyTable
	// Parameters : filePath - the path to the file to open
	// Return : unsigned int* - a dynamically allocated frequency table 
	// Notes : [256] contains the total count of characters in the file
	/////////////////////////////////////////////////////////////////////////////
	unsigned int* generateFrequencyTable(const char* filePath)
	{
		unsigned int* mTable = new unsigned int[257]{ 0 };

		ifstream mFin;
		mFin.open(filePath);
		char mBuffer;
		if (mFin.is_open())
		{
			
			while (true)
			{
				mBuffer = 0;
				mFin.read(&mBuffer, sizeof(mBuffer));

				if (mFin.eof())
					break;

				if (mBuffer != 0)
				{
					if (mBuffer == ' ')
						mBuffer = 32;

					++mTable[(int)mBuffer];
					++mTable[256];
				}
			}
			
		}
		mFin.close();


		return mTable;
		
		
	}
		/////////////////////////////////////////////////////////////////////////////
		// Function : generateLeafList
		// Parameters : freqTable - the frequency table to be used to generate leaf 
		//			nodes
		// Return : vector<HuffNode*> - a vector containing the leaf nodes
		/////////////////////////////////////////////////////////////////////////////
	vector<HuffNode*> generateLeafList(unsigned int* freqTable)
	{
		vector<HuffNode*> mLeafList;

		for (unsigned int i = 0; i < 256; ++i)
		{
			if (freqTable[i] > 0)
			{
				HuffNode* mTemp = new HuffNode;
				mTemp->left = nullptr;
				mTemp->right = nullptr;
				mTemp->parent = nullptr;
				mTemp->value = i;
				mTemp->freq = freqTable[i];
				

				mLeafList.push_back(mTemp);

			}
		}

		return mLeafList;
	}
		/////////////////////////////////////////////////////////////////////////////
		// Function : generateHuffmanTree
		// Parameters : leafList - the leaf nodes that will appear in our huffman 
		//			tree
		// Return : HuffNode* - the root of the generated tree
		/////////////////////////////////////////////////////////////////////////////
	HuffNode* generateHuffmanTree(vector<HuffNode*>& leafList)
	{
		priority_queue < HuffNode*, vector<HuffNode*>, cmp > mHuffmanTree;
		for (unsigned int i = 0; i < leafList.size(); ++i)
			mHuffmanTree.push(leafList[i]);

		while (mHuffmanTree.size() > 1)
		{
			HuffNode* mParent = new HuffNode;

			mParent->parent = nullptr;
			mParent->freq = 0;
			mParent->value = -1;

			mParent->left = mHuffmanTree.top();

			mParent->freq += mParent->left->freq;

			mHuffmanTree.pop();

			mParent->left->parent = mParent;

			mParent->right = mHuffmanTree.top();

			mParent->freq += mParent->right->freq;

			mHuffmanTree.pop();

			mParent->right->parent = mParent;

			mHuffmanTree.push(mParent);
		}
		return mHuffmanTree.top();
	}
		/////////////////////////////////////////////////////////////////////////////
		// Function : generateEncodingTable
		// Parameters : leafList - a vector containing all the leaves in the tree
		// Return : vector<int>* - a dynamically-allocated array of 256 vectors
		// Notes : each index in the encoding table corresponds to an index in the 
		//			frequency table
		/////////////////////////////////////////////////////////////////////////////
	vector<int>* generateEncodingTable(vector<HuffNode*>& leafList)
	{
		vector<int>* mTemp = new vector < int >[257];
		mTemp->reserve(leafList.size());
		HuffNode* mTempNode = new HuffNode;

		for (unsigned int i = 0; i < leafList.size(); i++)
		{
			mTempNode = leafList[i];
			int mIndex = leafList[i]->value;
			while (true)
			{
				if (mTempNode == mTempNode->parent->left)
					mTemp[mIndex].push_back(0);

				else if (mTempNode == mTempNode->parent->right)
					mTemp[mIndex].push_back(1);


				mTempNode = mTempNode->parent;

				if (mTempNode->parent == nullptr)
				{
					reverse(mTemp[mIndex].begin(), mTemp[mIndex].end());
					break;
				}
			}
		}

		return mTemp;
	}





		/////////////////////////////////////////////////////////////////////////////
		// Function : writeHuffmanFile
		// Parameters : 	inputFilePath - the path of the file to open for input
		//			outputFilePath - the path of the file to open for output
		//			freqTable - the frequency table
		//			encodingTable - the encoding table
		// Notes : open the input file, read characters one at a time, while 
		//		streaming bitcodes to the output file
		/////////////////////////////////////////////////////////////////////////////
	void writeHuffmanFile(const char* inputFilePath, const char* outputFilePath,
		unsigned int* freqTable, vector<int>* encodingTable)
	{

		BitOStream mFout(outputFilePath, (char*)freqTable, 1028);

		ifstream mFin;
		mFin.open(inputFilePath);


		if (mFin.is_open())
		{
			while (true)
			{
				if (mFin.eof())
					break;

				char mBuffer = 0;
				mFin.read(&mBuffer, 1);
				

				mFout << encodingTable[mBuffer];

			}

		}
		mFin.close();
		mFout.close();
	}
		/////////////////////////////////////////////////////////////////////////////
		// Function : cleanup
		// Notes : clean up any dynamic memory allocated by other functions
		/////////////////////////////////////////////////////////////////////////////
	void cleanup(unsigned int* freqTable, HuffNode* huffTree, vector<int>* encodingTable)
	{
		delete[] freqTable;
		PostOrder(huffTree);
		delete[] encodingTable;
	}

		/////////////////////////////////////////////////////////////////////////////
		// Function : decodeHuffmanFile
		// Parameters :	inputFilePath - the file to open and decode
		//			outputFilePath - the decoded mesasge, written to a file
		/////////////////////////////////////////////////////////////////////////////
	void decodeHuffmanFile(const char* inputFilePath, const char* outputFilePath)
	{
		unsigned int* mTable = new unsigned int[257]{ 0 };

		BitIStream mFin(inputFilePath, (char*)mTable, 1028);
	
		vector<HuffNode*> mLeafList = generateLeafList(mTable);

		HuffNode* mRoot = generateHuffmanTree(mLeafList);

		ofstream mFout;
		mFout.open(outputFilePath);

		HuffNode* mCurrent = mRoot;

		unsigned int mItems = mTable[255];

		while (mItems <= mTable[256])
		{
			if (mCurrent->value != -1)
			{
				char mBuffer = mCurrent->value;
				mFout << mBuffer;

				mItems++;
				mCurrent = mRoot;
			}

			int mHello = -1;
			mFin >> mHello;
			if (mHello == 0)
			{
				mCurrent = mCurrent->left;
			}
			else
			{
				mCurrent = mCurrent->right;
			}
		}
		mFin.close();
		mFout.close();


	}


