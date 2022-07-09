#pragma once

#include <string>
#include <cstdarg>
#include <conio.h>
#include <future>
#include <functional>
#include <Windows.h>
#include <iostream>
#include <list>
#include "UnicodeTextGenerator/UnicodeTextGenerator.h"
#include "../DynamicArrayFiles/DynamicArray.h"

#define ARROW_UP    72
#define ARROW_DOWN  80
#define ENTER       13

class DynamicMenu;

enum Type
{
	NormalEntry = 0, /* Normal entry with a function */
	SubMenuEntry = 1, /* an entry which contains a submenu */
	BooleanEntry = 2, /* an entry that has a boolean */
};

class MenuEntry
{
public:
	std::wstring Name;
	std::function<void()> Function;
	DynamicMenu *SubMenu;

	Type EntryType;

	bool* Boolean;

	MenuEntry(){}

	MenuEntry(std::wstring name, std::function<void()> function)
	{
		Name = name;
		Function = function;
		EntryType = NormalEntry;
	}

	MenuEntry(std::wstring name, DynamicMenu *subMenu)
	{
		Name = name;
		SubMenu = subMenu;
		EntryType = SubMenuEntry;
	}

	MenuEntry(std::wstring name, bool* boolean)
	{
		Name = name;
		Boolean = boolean;
		EntryType = BooleanEntry;
	}
};

class DynamicMenu
{
private:
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;
	DynamicArray<MenuEntry> MenuEntryList;
	bool ContinueMenu, AddExitEntry, CustomTitle;
	std::wstring Title;
public:
	
	/// <summary>
	/// creates the menu object
	/// </summary>
	/// <param name="title">- Title</param>
	/// <param name="customTitle">- if the title should be generated to UnicodeText or use the string direct</param>
	/// <param name="addExitEntry">- add a premade function to stop the menu</param>
	DynamicMenu(std::wstring title, bool customTitle = false, bool addExitEntry = true);

	/// <summary>
	/// Starts the menu
	/// </summary>
	void StartMenu();

	/// <summary>
	/// Draws the menu
	/// </summary>
	void DrawMenu(int CurrentIndex, int* TitleSize);

	std::wstring EntryString(int CurrentIndex, bool selected);

	/// <summary>
	/// Adds entry to menu
	/// </summary>
	/// <param name="Entry">- the entry to add</param>
	void AddMenuEntry(MenuEntry Entry);

	/// <summary>
	/// quits the menu
	/// </summary>
	void QuitMenu();
};