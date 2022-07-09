#pragma once

#include <string>
#include <cstdarg>
#include <conio.h>
#include <future>
#include <functional>
#include <Windows.h>
#include <iostream>
#include <list>

#define ARROW_UP    72
#define ARROW_DOWN  80
#define ENTER       13

class DynamicMenu;

enum Type
{
	Entry = 0, /* Normal entry with a function */
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
		EntryType = Entry;
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
	static CONSOLE_SCREEN_BUFFER_INFO csbi;
	static int columns, rows;
	static std::list<MenuEntry> MenuEntryList;
	static bool SetUpEntries, ContinueMenu;
public:
	/// <summary>
	/// Create Menu once everything was set up
	/// </summary>
	/// <param name="Title">Title to be displayed</param>
	/// <param name="AddExitEntry">if there should be "exit menu" entry at the bottom</param>
	static void CreateMenu(std::wstring Title);

	/// <summary>
	/// Add Entries to Menu
	/// </summary>
	/// <param name="size">Amount of entries being added</param>
	/// <param name="...">Entry Variable</param>
	static void AddMenuEntry(MenuEntry Entry);
};