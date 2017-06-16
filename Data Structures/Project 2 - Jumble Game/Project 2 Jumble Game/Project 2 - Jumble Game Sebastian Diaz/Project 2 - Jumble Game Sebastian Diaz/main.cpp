#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////
// LEAK DETECTION
////////////////////////////////////////////////////////////////////////////////////////////////
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <Windows.h>
#include <ctime>
#include <conio.h>
using namespace std;
#include <stdio.h>
#include "HTable.h"
#include "SLList.h"
#include "DynArray.h"
#include "BST.h"


////////////////////////////////////////////////////////////////////////////////////////////////
// CLASSES
////////////////////////////////////////////////////////////////////////////////////////////////
class DTSTimer
{
	unsigned int startTime;
public:
	DTSTimer()
	{
		reset();
	}

	void reset()
	{
		startTime = clock();
	}

	unsigned int getElapsedTime()
	{
		return (clock() - startTime);
	}

	static void delay(const unsigned int milli)
	{
		unsigned int st = clock();
		while (clock() - st < milli) {}
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////
// HASHFUNCTION
////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int HashFunction(const string& v)
{

	unsigned int mIndex = 10000169;

	for (unsigned int i = 0; i < v.length(); i++)
	{
		mIndex ^= ((mIndex << 5) + v[i] + (mIndex >> 2));
	}

	return mIndex % 600;
}

void insertion_sort(string mRow, int length) {
	int j, temp;

	for (int i = 0; i < length; i++) {
		j = i;

		while (j > 0 && mRow[j] < mRow[j - 1]) {
			temp = mRow[j];
			mRow[j] = mRow[j - 1];
			mRow[j - 1] = temp;
			j--;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////////////


const unsigned int mNumofHashBuckets = 600;
HTable<string> Dictionary(mNumofHashBuckets, HashFunction);
DynArray<string> mWordtoshuffle;
BST<string> WordsUser;


////////////////////////////////////////////////////////////////////////////////////////////////
// LOAD DICTIONARY
////////////////////////////////////////////////////////////////////////////////////////////////

void LoadDictionary()
{

	ifstream mInput;
	mInput.open("engldict.txt");
	if (mInput.is_open())
	{
		while (true)
		{
			string mRow;
			getline(mInput, mRow, '\n');
			mInput.ignore(INT_MAX, '\n');


			if (mRow.find('\'') == mRow.npos)
			{
				if (mRow.size() > 2 && mRow.size() < 7)
				{
					Dictionary.insert(mRow);
					if (mRow.size() == 6)
						mWordtoshuffle.append(mRow);
				}
			}
			if (mInput.eof())
				break;
		}
	}
	mInput.close();
}
bool checkword(string correct, string input)
{
	for (unsigned int i = 0; i < input.length(); i++)
	{
		char mWord = input[i];
		if (correct.find(mWord) == correct.npos)
		{
			return false;
		}
		correct.erase(correct.find(mWord), correct.find(mWord) + 1);

	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	// LEAK DETECTION
	////////////////////////////////////////////////////////////////////////////////////////////////
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// this function call will set a breakpoint at the location of a leaked block
	// set the parameter to the identifier for a leaked block
	//	_CrtSetBreakAlloc();

	////////////////////////////////////////////////////////////////////////////////////////////////

	LoadDictionary();
	Dictionary.printSomeStuff();
	unsigned int mScore = 0;

label:
	DTSTimer bob;
	srand((unsigned int)time(NULL));
	unsigned int mRandomNumber = rand();
	unsigned int mRandomIndex = mRandomNumber % mWordtoshuffle.size();
	string SixLetterWord = mWordtoshuffle.operator[](mRandomIndex);
	string CorrectWord = SixLetterWord;
	string CorrectWordCopy = SixLetterWord;

	string userInput;
	random_shuffle(SixLetterWord.begin(), SixLetterWord.end());
	bool Correct = false;
	
	while (bob.getElapsedTime() < 60000)
	{
		LockWindowUpdate(GetConsoleWindow());
		// 1 - get user input
		if (_kbhit())
		{
			char ch = _getch();

			if (-32 == ch)
				_getch();

			// did the user press a-z?
			else if (isalpha(ch))
			{
				userInput += tolower(ch);
			}
		}

		// 2 - act on it
		if (GetAsyncKeyState(VK_ESCAPE))
			break;

		if (GetAsyncKeyState(VK_RETURN))
		{
			if (WordsUser.find(userInput) == false && userInput.size() > 2 && userInput.size() < 7 && Dictionary.find(userInput) != -1)
			{
				if (checkword(CorrectWordCopy, userInput) == true)
				{

					if (userInput.size() == 3)
					{
						mScore = mScore + (3 * 2 * 1) -1;
					}
					else if (userInput.size() == 4)
					{
						mScore = mScore + (4 * 3 * 2 * 1) - 1;
					}
					else if (userInput.size() == 5)
					{
						mScore = mScore + (5 * 4 * 3 * 2 * 1) - 1;
					}
					else if (userInput.size() == 6)
					{
						Correct = true;
						mScore = mScore + (6 * 5 * 4 * 3 * 2 * 1) - 1;
					}
					WordsUser.insert(userInput);

				}
			}
			userInput.clear();
		}
		if (GetAsyncKeyState(VK_BACK))
		{
			if (userInput.size() != 0)
			{
				userInput.erase(userInput.end() - 1, userInput.end());
			}
		}
		
		// 3 - draw the screen
		system("cls");
		cout << "\tWelcome to Jumble Game\n";
		cout << SixLetterWord << '\n';
		WordsUser.printInOrder();
		cout << "\nYour Input Here: " << userInput << '\n';
		cout << "Time Left: " << 60 - bob.getElapsedTime() / 1000 << '\n';
		cout << "Your Score: " << mScore << '\n';
		LockWindowUpdate(NULL);
		bob.delay(100);
		
	}

	if (Correct == true)
	{
		bob.reset();
		WordsUser.clear();
		goto label;
	}

		cout << "\nThe six letter word was: " << CorrectWord << '\n';

		string answer;
		cout << "Do you want to retry? you'll looseyour score: type Yes or No: ";
		getline(cin, answer, '\n');
		if (answer == "yes")
		{
			mScore = 0;
			goto label;
		}
		else
		{
			string name;
			cout << "\nPlease enter your name: ";
			getline(cin, name, '\n');

			//Save
			fstream  fout;
			fout.open("saveScores.txt", ios_base::out | ios_base::app); //Defaults to truncate


			if (fout.is_open())
			{
				fout << name << '\t' << mScore << '\n';
				fout.close();
			}


			ifstream fin;
			string GetLastScore;
			int recordCount = 0;

			fin.open("saveScores.txt");

			if (fin.is_open())
			{

				int i = 0;
				cout << "\t\t\t\tHigh Score\n";
				while (true)
				{

					if (fin.eof())
						break;
					getline(fin, GetLastScore);
					cout << "\t\t\t\t"<< GetLastScore << '\n';

					++recordCount;
				}

				insertion_sort(GetLastScore, GetLastScore.length());

				fin.close();
			}


		}

		string answer2;
		cout << "Do you want to play again? Yes or No: ";
		getline(cin, answer2, '\n');
		if (answer2 == "yes")
		{
			mScore = 0;
			goto label;
		}

		




	return 0;
};
