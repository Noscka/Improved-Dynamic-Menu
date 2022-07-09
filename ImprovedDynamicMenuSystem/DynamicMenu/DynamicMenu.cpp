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

void DynamicMenu::CreateMenu()
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

	int c, ex, counter = 0, CurrentIndex = 0;

	while (ContinueMenu)
	{
		std::wstring OutputString; // string for full "display" as it is the most perfomace efficent method

		if (CustomTitle) /* If custom Title is true, its going to use the straight characters instead of generating a unicode title*/
			OutputString = Title;
		else
			OutputString = AsciiTextGenerator::UnicodeTitleGenerate(Title); // add title with "ascii generator"

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		// for loop using counter to get the index so to add the >< to the selected option
		counter = 0;
		for (MenuEntry Entry : MenuEntryList)
		{
			/* Do different things for NormalEntry and Submenu Entry*/
			switch (Entry.EntryType)
			{
			case NormalEntry:
				// Append to string as to make it be 1 print operation, makes it way quicker
				if (counter == CurrentIndex)
				{
					OutputString += std::wstring((columns / 2) - (1 + Entry.Name.length()) / 2, ' ') + L">" + Entry.Name + L"<\n";
				}
				else
				{
					OutputString += std::wstring((columns / 2) - Entry.Name.length() / 2, ' ') + Entry.Name + L'\n';
				}
				break;
			case SubMenuEntry:
				// Append to string as to make it be 1 print operation, makes it way quicker
				if (counter == CurrentIndex)
				{
					OutputString += std::wstring((columns / 2) - (1 + Entry.Name.length()) / 2, ' ') + L"\033[103m" + L"\033[30m" + L">" + Entry.Name + L"<\033[0m" + L'\n';
				}
				else
				{
					OutputString += std::wstring((columns / 2) - Entry.Name.length() / 2, ' ') + L"\033[103m" + L"\033[30m" + Entry.Name + L"\033[0m" + L'\n';
				}
				break;
			}
			counter++;
		}

		wprintf(OutputString.c_str());

		// get key input for arrow and enter inputs
		c = _getch();

		if (c == ENTER)
		{
			/* Get the entry from the list */
			//std::list<MenuEntry>::iterator iterator = MenuEntryList.begin();
			//std::advance(iterator, CurrentIndex);

			switch (MenuEntryList[CurrentIndex].EntryType)
			{
			case NormalEntry:
				clear_screen();
				MenuEntryList[CurrentIndex].Function();
				break;

			case SubMenuEntry:
				clear_screen();
				MenuEntryList[CurrentIndex].SubMenu->CreateMenu();
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

		clear_screen();
	}
}

void DynamicMenu::AddMenuEntry(MenuEntry Entry)
{
	MenuEntryList.Append(Entry);
}