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

#pragma region Variables
CONSOLE_SCREEN_BUFFER_INFO DynamicMenu::csbi;
HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
int DynamicMenu::columns, DynamicMenu::rows;
std::list<MenuEntry> DynamicMenu::MenuEntryList;
bool DynamicMenu::SetUpEntries, DynamicMenu::ContinueMenu = true;
#pragma endregion


void DynamicMenu::CreateMenu(std::wstring Title)
{
	DynamicMenu::ContinueMenu = true; // incase menu was quit before

	//if (AddExitEntry)
	//{
	//	DynamicMenu::MenuEntryList.push_back(MenuEntry("Quit Menu", QuitMenu));
	//}

	int c, ex, counter = 0, CurrentIndex = 0;

	// only run is entries were added
	if (SetUpEntries)
	{
		while (ContinueMenu)
		{
			std::wstring OutputString; // string for full "display" as it is the most perfomace efficent method

			//OutputString = AsciiTextGenerator::AsciiWrite(Title); // add title with "ascii generator"

			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

			// for loop using counter to get the index so to add the >< to the selected option
			counter = 0;
			for (MenuEntry Entry : DynamicMenu::MenuEntryList)
			{
				// Append to string as to make it be 1 print operation, makes it way quicker
				if (counter == CurrentIndex)
				{
					OutputString += std::wstring((columns / 2) - (1 + Entry.Name.length()) / 2, ' ') + L">" + Entry.Name + L"<\n";
				}
				else if (Entry.EntryType == SubMenuEntry)
				{
					OutputString += std::wstring((columns / 2) - Entry.Name.length() / 2, ' ') + L"\033[103m" + L"\033[30m" + Entry.Name + L"\033[0m" + L"\n";
				}
				else
				{
					OutputString += std::wstring((columns / 2) - Entry.Name.length() / 2, ' ') + Entry.Name + L"\n";
				}
				counter++;
			}

			wprintf(OutputString.c_str());

			// get key input for arrow and enter inputs
			c = _getch();

			if (c == ENTER)
			{
				// if enter, get the function of the selected entry and run it
				std::list<MenuEntry>::iterator iterator = DynamicMenu::MenuEntryList.begin();
				std::advance(iterator, CurrentIndex);

				//Functions::ClearScreen();
				((MenuEntry)*iterator).Function();

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
					if (CurrentIndex < DynamicMenu::MenuEntryList.size() - 1) // Increment only if larger the 0
					{
						CurrentIndex++; // Increment the Indenetation
					}
					break;
				}
			}

			clear_screen();
		}
	}
	else
	{
		throw std::invalid_argument("No Entries Added");
	}
}

void DynamicMenu::AddMenuEntry(MenuEntry Entry)
{
	SetUpEntries = true; // allow for "CreateMenu" to run

	MenuEntryList.push_back(Entry);
}