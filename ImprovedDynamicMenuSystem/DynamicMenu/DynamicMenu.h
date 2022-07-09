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

#define ARROW_UP    72
#define ARROW_DOWN  80
#define ENTER       13

class DynamicMenu;

enum Type
{
	NormalEntry = 0, /* Normal entry with a function */
	SubMenuEntry = 1, /* an entry which contains a submenu */
};

class MenuEntry
{
public:
	std::wstring Name;
	std::function<void()> Function;
	DynamicMenu *SubMenu;

	Type EntryType;

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
};

class DynamicMenu
{
private:
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;
	std::list<MenuEntry> MenuEntryList;
	bool SetUpEntries, ContinueMenu, AddExitEntry, CustomTitle;
	std::wstring Title;
public:
	/// <summary>
	/// Constructer for the menu
	/// </summary>
	/// <param name="Title">Title/name of the menu</param>
	DynamicMenu(std::wstring title, bool customTitle = false, bool addExitEntry = true);

	/// <summary>
	/// Create Menu once everything was set up
	/// </summary>
	/// <param name="Title">Title to be displayed</param>
	/// <param name="AddExitEntry">if there should be "exit menu" entry at the bottom</param>
	void CreateMenu();

	/// <summary>
	/// Add Entries to Menu
	/// </summary>
	/// <param name="size">Amount of entries being added</param>
	/// <param name="...">Entry Variable</param>
	void AddMenuEntry(MenuEntry Entry);

	void QuitMenu();
};