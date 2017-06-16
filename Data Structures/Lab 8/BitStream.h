#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
class BitOStream
{
	ofstream  fout;
	int mCurrent;
	char mBuffer;
public:
	/////////////////////////////////////////////////////////////////////////////// Function : constructor
	// Parameters : 	filePath - the path of the file to open for output
	//			headerChunk - a chunk of data to be written at the 
	//					beginning of the file
	//			numberOfBytes - the number of bytes of header information 
	//					to write out
	/////////////////////////////////////////////////////////////////////////////
	BitOStream(const char* filePath, const char* headerChunk = NULL, int numberOfBytes = 0)
	{
		mCurrent = 0;
		mBuffer = 0;

		fout.open(filePath, ios_base::binary);

		if (headerChunk)
		{
			fout.write(headerChunk, numberOfBytes);
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	// Function : destructor
	/////////////////////////////////////////////////////////////////////////////
	~BitOStream()
	{
		close();
	}
	/////////////////////////////////////////////////////////////////////////////
	// Function : insertion operator
	// Parameters : bits - a vector containing some number of 1's and 0's to 
	//						stream out to the file
	// Return : BitOStream& - the stream (allows for daisy-chaining insertions)
	////////////////////////////////////////////////////////////////////////////
	BitOStream& operator<<(vector<int> &bits)
	{
		for (unsigned int i = 0; i < bits.size(); ++i)
		{
			if (bits[i] == 1)
			{
				mBuffer |= (128 >> mCurrent);
			}
			mCurrent++;
			if (mCurrent == 8)
			{
				fout.write((char*)&mBuffer, 1);
				mCurrent = 0;
				mBuffer = 0;
			}
		}
		return *this;
	}
	/////////////////////////////////////////////////////////////////////////////
	// Function : close
	// Notes : closes the file stream - if remaining bits exist, they are written
	//			to the file with trailing 0's. if no remaining bits exist, 
	//			simply close the file
	/////////////////////////////////////////////////////////////////////////////
	void close()
	{
		if (mCurrent != 0)
			fout.write(&mBuffer, 1);

		fout.close();
	}
};


class BitIStream
{
	ifstream fin;
	int mCurrent;
	char mBuffer;
public:
	/////////////////////////////////////////////////////////////////////////////
	// Function : constructor
	// Parameters : 	filePath - the path of the file to open for input
	//			headerChunk - a chunk of data to be read from the 
	//					beginning of the file
	//			numberOfBytes - the number of bytes of header information 
	//					to read in
	/////////////////////////////////////////////////////////////////////////////
	BitIStream(const char* filePath, char* headerChunk = NULL, int numberOfBytes = 0)
	{
		

		fin.open(filePath, ios_base::binary);

			if (headerChunk != NULL)
			{
				fin.read(headerChunk, numberOfBytes);
			}

			mCurrent = 0;
			mBuffer = 0;

	}
	/////////////////////////////////////////////////////////////////////////////
	// Function : destructor
	/////////////////////////////////////////////////////////////////////////////
	~BitIStream()
	{
		close();
	}
	/////////////////////////////////////////////////////////////////////////////
	// Function : extraction operator
	// Parameters : bit - store the next bit here
	// Return : BitIStream& - the stream (allows for daisy-chaining extractions)
	/////////////////////////////////////////////////////////////////////////////
	BitIStream& operator>>(int &bit)
	{

			if (!mBuffer || mCurrent == 8)
			{
				fin.read((char*)&mBuffer, 1);
				mCurrent = 0;
			}

			bit = mBuffer & (128 >> mCurrent) ? 1 : 0;
			mCurrent++;

		return *this;
	}
	/////////////////////////////////////////////////////////////////////////////
	// Function : eof
	// Return : true if we are at the end of the file, false otherwise
	// Notes : should only return true when we have given the user every byte
	//		from the file and every bit from the buffer
	/////////////////////////////////////////////////////////////////////////////
	bool eof()
	{
		if (!fin.eof())
			return false;
		else
			return true;

	}
	/////////////////////////////////////////////////////////////////////////////
	// Function : close
	// Notes : close the file
	/////////////////////////////////////////////////////////////////////////////
	void close()
	{
		fin.close();
	}


};

