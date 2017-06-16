#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <Windows.h>
#include <ctime>
#include <conio.h>
#include "MenuManager.h"
#include "DynArray.h"

using namespace std;
int mItems;
MenuManager mManager;
menu mMainMenu;

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
menu Loadmenu(string File)
{
	ifstream mInput;
	mInput.open(File);

	if (mInput.is_open())
	{
		mMainMenu.ClearChoices();
		//something to parse through
		string mRow;
		string mTitle;
		//make these get line
		getline(mInput, mTitle, '\n');
		mInput >> mItems;
		mInput.ignore(INT_MAX, '\n');
		mMainMenu.SetTitle(mTitle);

		for (int i = 0; i < mItems; i++)
		{
			string mOtherData;
			menuItem mTemp;

			mInput >> mOtherData;

			getline(mInput, mRow);

			mRow.erase(0, mRow.find("\"") + 1);
			mTemp.name = mRow.substr(0, mRow.find('\"'));
			mRow.erase(0, mRow.find('\"') + 3);
			//check if its a menu or option
			if (mOtherData == "MENU")
			{
				mTemp.is_subMenu = true;
				mTemp.subMenuPath = mRow.substr(0, mRow.find('\"'));
				mRow.erase(0, mRow.find('\"') + 1);

			}

			mMainMenu.AppendMenuItem(mTemp);
		}
		mInput.close();

	}
	mManager.GetActiveMenu().addHead(mMainMenu);

	return mMainMenu;
};

int DisplaytoScreen(menu  MenuSelected, int Cursor)
{

	cout << '\t' << MenuSelected.GetTitle() << '\n';
	int i = 0;
	for (; i < MenuSelected.GetMenu().size(); i++)
	{


		cout << ((Cursor == i && Cursor != MenuSelected.GetMenu().size()) ? '>' : ' ') << "\t" << MenuSelected.GetMenu()[i].name << '\n';


	}
	cout << ((Cursor == i) ? '>' : ' ') << '\t' << "Exit";

	return i;
};
int main(int argc, char ** argv)
{
	DTSTimer bob;
	int mEndofMenu = mMainMenu.GetMenu().size();
	int mCursor = 0;

	Loadmenu("main.mnu");
	DisplaytoScreen(mMainMenu, mCursor);

	while (true)
	{

		if (GetAsyncKeyState(VK_ESCAPE))
			break;
		if (GetAsyncKeyState(VK_UP))
		{
			if (mCursor >= 1)
				--mCursor;
			else if (mCursor < 1)
				mCursor = mEndofMenu;
		}
		if (GetAsyncKeyState(VK_DOWN))
		{

			if (mCursor < mEndofMenu)
				++mCursor;
			else
				mCursor = 0;
		}
		if (GetAsyncKeyState(VK_RETURN))
		{

			if (mCursor == mMainMenu.GetMenu().size())
			{
				if (mManager.GetActiveMenu().size() <= 1)
					break;
				
					mCursor = mMainMenu.GetCurrent();
					SLLIter<menu> Menutodelete(mManager.GetActiveMenu());
					Menutodelete.begin();
					mManager.GetActiveMenu().remove(Menutodelete);
					mMainMenu = Menutodelete.current();
				
			}
			 else if (mMainMenu.GetMenu()[mCursor].is_subMenu == true)
			{
				mMainMenu.SetCurrent(mCursor);
				menu NewMenu = Loadmenu(mMainMenu.GetMenu()[mCursor].subMenuPath);
				mCursor = 0;

			}
		}
		bob.delay(200);
		system("cls");
		mEndofMenu = DisplaytoScreen(mMainMenu, mCursor);
	}

	return 0;
};



