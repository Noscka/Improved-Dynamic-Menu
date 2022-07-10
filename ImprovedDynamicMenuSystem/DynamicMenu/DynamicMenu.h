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
#define ARROW_LEFT	75
#define ARROW_RIGHT	77

#define ENTER       13
#define BACKSPACE	8

class DynamicMenu;

enum Type
{
	NormalEntry = 0, /* Normal entry with a function */
	SubMenuEntry = 1, /* an entry which contains a submenu */
	BooleanEntry = 2, /* an entry that has a boolean */
	IntegerEntry = 3, /* an entry that has a Integer */
};

class MenuEntry
{
public:
	// Var across all types
	std::wstring Name;
	Type EntryType;

	// Type specific vars
	std::function<void()> Function;
	DynamicMenu* SubMenu;
	bool* Boolean;
	int* Integer;

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

	MenuEntry(std::wstring name, int* integer)
	{
		Name = name;
		Integer = integer;
		EntryType = IntegerEntry;
	}
};

class DynamicMenu
{
private:
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE ConsoleHandle;
	int columns, rows;
	DynamicArray<MenuEntry> MenuEntryList;
	bool ContinueMenu, AddExitEntry, CustomTitle, CenteredTitle, AddedQuit;
	std::wstring Title;
public:
	
	/// <summary>
	/// creates the menu object
	/// </summary>
	/// <param name="title">- menu title</param>
	/// <param name="customTitle">- if the title should be generated to UnicodeText or use the string direct</param>
	/// <param name="addExitEntry">- add a premade function to stop the menu</param>
	/// <param name="centeredTitle">- If the title should be centered or not</param>
	DynamicMenu(std::wstring title, bool customTitle = false, bool addExitEntry = true, bool centeredTitle = true);

	/// <summary>
	/// Starts the menu
	/// </summary>
	void StartMenu();

	/// <summary>
	/// Draws the menu
	/// </summary>
	/// <param name="CurrentIndex">- currrent index</param>
	/// <param name="TitleSize">- pointer to the title size int so it can be calculated</param>
	void DrawMenu(int CurrentIndex, int* TitleSize);

	/// <summary>
	/// Returns line string for a entry
	/// </summary>
	/// <param name="EntryIndex">- the index of the entry wanted</param>
	/// <param name="selected">- if the entry is selected or not</param>
	/// <returns>Entry line string</returns>
	std::wstring EntryString(int EntryIndex, bool selected);

	/// <summary>
	/// Clears the whole screen
	/// </summary>
	/// <param name="fill">- what character to fill the screen with</param>
	void clear_screen(char fill = ' ');

	/// <summary>
	/// Clears the line selected
	/// </summary>
	/// <param name="Position">- The absolute position</param>
	void ClearCurrentLine(int Position);

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