#pragma once
#include <iostream>
#include <string>
#include "DynArray.h"
#include "SLList.h"

using namespace std;

struct menuItem
{
	std::string name;
	bool is_subMenu = false;
	std::string subMenuPath;
};

class menu
{
	string title;
	DynArray<menuItem> choices;
	unsigned int currSelection;
public:
	bool operator == (const menu& rhs) const
	{
		return title == rhs.title;
	}
	unsigned int GetCurrent()
	{
		return currSelection;
	}
	void SetCurrent(unsigned int  curr)
	{
		currSelection = curr;
	}
	DynArray<menuItem> GetMenu()
	{
		return choices;
	}
	void ClearChoices()
	{
		choices.clear();
	}
	string GetTitle()
	{
		return title;
	}
	void AppendMenuItem(menuItem add)
	{
		choices.append(add);
	}
	void SetTitle(string name)
	{
		title = name;
	}
	void SetMenuItem(string name, bool issubmenu, string path = "")
	{
		menuItem mTemp;
		mTemp.name = name;
		mTemp.is_subMenu = issubmenu;
		mTemp.subMenuPath = path;
		AppendMenuItem(mTemp);
	}
	void SetCurrSelected(int selected)
	{
		currSelection = selected;
	}

};

class MenuManager
{
	SLList<menu> activeMenus;
public:
	SLList<menu>& GetActiveMenu()
	{
		return activeMenus;
	}
};


