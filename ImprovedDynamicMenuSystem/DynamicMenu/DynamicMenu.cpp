#include "DynamicMenu.h"

void clear_screen(char fill = ' ')
{
	COORD tl = { 0,0 };
	CONSOLE_SCREEN_BUFFER_INFO s;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console, &s);
	DWORD written, cells = s.dwSize.X * s.dwSize.Y;
	FillConsoleOutputCharacter(console, fill, cells, tl, &written);
	FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(console, tl);
}

DynamicMenu::DynamicMenu(std::wstring title, bool customTitle, bool addExitEntry)
{
	Title = title;
	AddExitEntry = addExitEntry;
	CustomTitle = customTitle;

	MenuEntryList = DynamicArray<MenuEntry>(3, 2);
}

void DynamicMenu::QuitMenu()
{
	ContinueMenu = false;
}

void DynamicMenu::DrawMenu(int CurrentIndex, int* TitleSize)
{
	clear_screen();

	std::wstring OutputString; // string for full "display" as it is the most perfomace efficent method

	if (CustomTitle) /* If custom Title is true, its going to use the straight characters instead of generating a unicode title*/
		OutputString = Title;
	else
		OutputString = AsciiTextGenerator::UnicodeTitleGenerate(Title); // add title with "ascii generator"

	for (int i = 0; i < OutputString.size(); i++)
	{
		if (OutputString[i] == '\n')
		{
			(*TitleSize)++;
		}
	}

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	// for loop using counter to get the index so to add the >< to the selected option
	for (int i = 0; i < MenuEntryList.ArrayIndexPointer; i++)
	{
		if (i == CurrentIndex)
			OutputString += EntryString(i, true);
		else
			OutputString += EntryString(i, false);
	}

	wprintf(OutputString.c_str());
}

std::wstring DynamicMenu::EntryString(int EntryIndex, bool selected)
{
	int SpaceLenght = ((columns / 2) - MenuEntryList[EntryIndex].Name.length() / 2);
	std::wstring output;

	switch (MenuEntryList[EntryIndex].EntryType)
	{
	case NormalEntry:
		// Append to string as to make it be 1 print operation, makes it way quicker
		if (selected)
		{
			output += std::wstring(SpaceLenght - 2, ' ') + L">>" + MenuEntryList[EntryIndex].Name + L"<<\n";
		}
		else
		{
			output += std::wstring(SpaceLenght, ' ') + MenuEntryList[EntryIndex].Name + L"\n";
		}
		break;
	case SubMenuEntry:
		// Append to string as to make it be 1 print operation, makes it way quicker
		if (selected)
		{
			output += std::wstring(SpaceLenght - 2, ' ') + L"\033[36m>>" + MenuEntryList[EntryIndex].Name + L"<<\033[0m\n";
		}
		else
		{
			output += std::wstring(SpaceLenght, ' ') + L"\033[36m" + MenuEntryList[EntryIndex].Name + L"\033[0m\n";
		}
		break;

	case BooleanEntry:
		std::wstring FullBoolText = MenuEntryList[EntryIndex].Name + std::wstring(4, ' ') + L"[ ]";

		if (selected)
		{
			output += std::wstring(SpaceLenght - 2, ' ') + L"\033[36m>>" + FullBoolText + L"<<\033[0m\n";
		}
		else
		{
			output += std::wstring(SpaceLenght, ' ') + L"\033[36m" + FullBoolText + L"\033[0m\n";
		}
		break;
	}

	return output;
}

void DynamicMenu::StartMenu()
{
	ContinueMenu = true; // incase menu was quit before

	if (AddExitEntry)
	{
		std::function<void()> Func = [this]()
		{
			return this->QuitMenu();
		};

		MenuEntryList.Append(MenuEntry(L"Quit", Func));
	}

	int c, ex;
	int CurrentIndex = 0; /* Which item is currently selected */
	int OldIndex = CurrentIndex; /* Old index to know old position */
	int TitleSize = 0; /* title size (for calculations where actual menu entries start) */

	int LastMenuSize = MenuEntryList.ArrayIndexPointer; /* for checking if the menu has increased/descreased */

	DrawMenu(CurrentIndex, &TitleSize); /* Draw menu first time*/

	while (ContinueMenu)
	{
		// get key input for arrow and enter inputs
		c = _getch();

		if (c == ENTER)
		{

			switch (MenuEntryList[CurrentIndex].EntryType)
			{
			case NormalEntry:
				clear_screen();
				MenuEntryList[CurrentIndex].Function();
				break;

			case SubMenuEntry:
				clear_screen();
				MenuEntryList[CurrentIndex].SubMenu->StartMenu();
				break;
			}
		}
		else if (!(c && c != 224))
		{
			// if not enter, do second getch as arrows input 2 characters to show they are arrows
			switch (ex = _getch())
			{
			case ARROW_UP:
				if (CurrentIndex > 0) // Decrement only if smaller then List size
				{
					CurrentIndex--; // Decrement the Indenetation
				}
				break;

			case ARROW_DOWN:
				if (CurrentIndex < MenuEntryList.GetArrayIndexPointer() - 1) // Increment only if larger the 0
				{
					CurrentIndex++; // Increment the Indenetation
				}
				break;
			}
		}

		if (LastMenuSize != MenuEntryList.ArrayIndexPointer)
		{
			DrawMenu(CurrentIndex, &TitleSize); /* Redraw menu to include new entries */
			LastMenuSize = MenuEntryList.ArrayIndexPointer;
		}
		else if (CurrentIndex == OldIndex)
			continue;
		else
		{
			COORD tl;
			if(CurrentIndex > OldIndex)
				tl = { 0, (SHORT)(TitleSize + CurrentIndex-1) };
			else
				tl = { 0, (SHORT)(TitleSize + CurrentIndex) };
			
			CONSOLE_SCREEN_BUFFER_INFO s;
			HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
			GetConsoleScreenBufferInfo(console, &s);
			DWORD written, cells = s.dwSize.X * 2;
			FillConsoleOutputCharacter(console, ' ', cells, tl, &written);
			FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);

			/*
			What needs to be redrawing depending on if its up for down
			if the index goes down (bigger number), you need to clear above and current line
			
			|| Old Selected Entry
			\/ New Selected Entry <-- Here is Cursor

			and if going up (smaller number)
			/\ New Selected Entry <-- Here is Cursor
			|| Old Selected Entry
			*/

			if (CurrentIndex > OldIndex)
			{
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, (SHORT)(TitleSize + CurrentIndex-1) });
				wprintf((EntryString(OldIndex, false) + EntryString(CurrentIndex, true)).c_str());
			}
			else
			{
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, (SHORT)(TitleSize + CurrentIndex) });
				wprintf((EntryString(CurrentIndex, true) + EntryString(OldIndex, false)).c_str());
			}

		}

		OldIndex = CurrentIndex;
	}
}

void DynamicMenu::AddMenuEntry(MenuEntry Entry)
{
	MenuEntryList.Append(Entry);
}