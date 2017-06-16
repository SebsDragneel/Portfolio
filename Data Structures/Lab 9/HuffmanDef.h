////////////////////////////////////////////////////////////////////////////////////////////////
// File :			HuffmanDef.h
// Author :			Rodney Stoeffler
// Last Modified :	03.24.08
// Purpose :		Definition file and Prototypes for the Huffman Utilities.
// Notes :			Property of Full Sail University, Data Structures Course.
////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDE PROTECTION
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _HUFFMANDEF_H_
#define _HUFFMANDEF_H_


////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <queue>
using namespace std;


////////////////////////////////////////////////////////////////////////////////////////////////
// Structure :	HuffNode
// Members :	value - the value of the Node (-1, 0-255)
//				freq - the frequency of the Node
//				left - pointer to the left child Node
//				right - pointer to the right child Node
//				parent - pointer to the parent Node
////////////////////////////////////////////////////////////////////////////////////////////////
struct HuffNode
{
	short value;
	unsigned int freq;
	HuffNode *left, *right, *parent;
};


////////////////////////////////////////////////////////////////////////////////////////////////
// Function :	showFrequencyTable
// Parameters :	freqTable - the frequency table to show
// Return :		void
////////////////////////////////////////////////////////////////////////////////////////////////
void showFrequencyTable (unsigned int *freqTable)
{
	cout << "Freq Table\n";
	for (int i = 0; i < 256; ++i)
		if (freqTable[i]) cout << (char)i << "(" << i << ")  " << " : " << freqTable[i] << '\n';
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Function :	showHuffmanTree
// Parameters :	root - the root of the tree to be shown
// Return :		void
////////////////////////////////////////////////////////////////////////////////////////////////
void showHuffmanTree (HuffNode *root)
{
	queue<HuffNode *> nodeQ;
	queue<int> depthQ;
	nodeQ.push (root);
	depthQ.push (0);
	int curDepth = 0;
	cout << "Depth 0 : ";
	while (!nodeQ.empty ())
	{
		HuffNode *curNode = nodeQ.front ();
		int depth = depthQ.front ();
		nodeQ.pop (); depthQ.pop ();
		if (curNode->left)
		{
			nodeQ.push (curNode->left);
			depthQ.push (depth + 1);
		}
		if (curNode->right)
		{
			nodeQ.push (curNode->right);
			depthQ.push (depth + 1);
		}
		if (depth > curDepth)
		{
			curDepth = depth;
			cout << '\n';
			cout << "Depth " << curDepth << " : ";
		}
		if (curNode->value != -1)
			cout << (char)curNode->value << " ";
	}
	cout << '\n';
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Function :	showEncodingTable
// Parameters :	encodingTable - pointer to the encoding table.
// Return :		void
////////////////////////////////////////////////////////////////////////////////////////////////
void showEncodingTable (vector<int> *encodingTable)
{
	cout << "Encoding Table\n";
	for (unsigned int i = 0; i < 256; ++i)
		if (encodingTable[i].size () > 0)   // only show used entries?
		{
			cout << "Letter : " << (char)i << "\tCode : ";
			for (unsigned int j = 0; j < encodingTable[i].size (); ++j)
				cout << ((!encodingTable[i][j]) ? "0" : "1");
			cout << '\n';
		}
		cout << '\n';
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Function :	dispBin
// Parameters :	byte - the byte to be displayed in binary
// Return :		void
////////////////////////////////////////////////////////////////////////////////////////////////
void dispBin (unsigned char byte)
{
	for (int i = 7; i >= 0; --i)
		cout << ((byte & (1 << i)) ? 1 : 0);
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Function :	showHuffmanFileStream
// Parameters :	filePath - file to display
// Return :		void
////////////////////////////////////////////////////////////////////////////////////////////////
void showHuffmanFileStream (const char *filePath)
{
	// Load the Huffman File, skipping the first 1024 bytes.
	ifstream in (filePath, ios::binary);
	in.seekg (1024, ios::cur);

	unsigned int size;
	unsigned int compSize = 0;
	in.read ((char *)&size, 4);
	cout << "Final Huffman Encoded Data :\n";
	
	// Display the remaining info bit by bit.
	unsigned char ch;
	while (true)
	{
		in.read ((char *)&ch, 1);
		if (in.eof ())
			break;
		++compSize;
		static int curByte = 0;
		dispBin (ch);
		cout << ' ';
		if (++curByte == 8) { curByte = 0; cout << '\n'; }
	}      
	cout << "Huffman Uncompressed Data Size : " << size << '\n';
	cout << "Huffman Compressed Data Size : " << compSize << '\n';
}


////////////////////////////////////////////////////////////////////////////////////////////////
// END INCLUDE PROTECTION
////////////////////////////////////////////////////////////////////////////////////////////////
#endif