#include "DynamicMenu.h"

DynamicMenu::DynamicMenu(std::wstring title, bool customTitle, bool addExitEntry, bool centeredTitle)
{
	Title = title;
	AddExitEntry = addExitEntry;
	CustomTitle = customTitle;
	CenteredTitle = centeredTitle;

	AddedQuit = false;

	MenuEntryList = DynamicArray<MenuEntry>(3, 2);
	ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

void DynamicMenu::StartMenu()
{
	ContinueMenu = true; // incase menu was quit before

	if (AddExitEntry && !AddedQuit)
	{
		AddedQuit = true;
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
			case FunctionEntry:
				clear_screen();
				MenuEntryList[CurrentIndex].Function();
				DrawMenu(CurrentIndex, &TitleSize);
				break;

			case SubMenuEntry:
				clear_screen();
				MenuEntryList[CurrentIndex].SubMenu->StartMenu();
				DrawMenu(CurrentIndex, &TitleSize);
				break;

			case BooleanEntry:
				*MenuEntryList[CurrentIndex].Boolean = !*MenuEntryList[CurrentIndex].Boolean;

				ClearCurrentLine(TitleSize + CurrentIndex);

				wprintf(EntryString(CurrentIndex, true).c_str());
				break;

			case IntegerEntry:
			{
				std::wstring NewInt;
				wchar_t c;
				bool ContinueIntType = true;

				COORD NumberPosition = { (((columns / 2) - MenuEntryList[CurrentIndex].Name.length() / 2) + MenuEntryList[CurrentIndex].Name.length() + 3), (CurrentIndex + TitleSize) };

				SetConsoleCursorPosition(ConsoleHandle, NumberPosition);

				while (ContinueIntType)
				{
					c = _getch();

					if (c == ENTER)
					{
						ContinueIntType = false;
					}
					else if (c == BACKSPACE)
					{
						// Coord for backspace cursor position editing
						COORD NewCoord;

						if (!NewInt.empty())
						{
							GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);

							NewCoord = { (SHORT)(csbi.dwCursorPosition.X - 1), csbi.dwCursorPosition.Y }; // create new coord with x-1 and same y
							SetConsoleCursorPosition(ConsoleHandle, NewCoord); // use new coord
							wprintf(L" "); // delete character
							SetConsoleCursorPosition(ConsoleHandle, NewCoord);
							NewInt.pop_back();
						}
					}
					else if (isdigit(c) || (c == '-' && NewInt.empty()))
					{
						wprintf(L"%c", c);
						NewInt += c;
					}
				}

				if (!NewInt.empty())
				{
					try
					{
						*MenuEntryList[CurrentIndex].Integer = std::stoi(NewInt);
					}
					catch (std::out_of_range ex)
					{
						if(NewInt[0] == '-')
							*MenuEntryList[CurrentIndex].Integer = INT_MIN;
						else
							*MenuEntryList[CurrentIndex].Integer = INT_MAX;
					}
				}
				ClearCurrentLine(TitleSize + CurrentIndex);
				wprintf(EntryString(CurrentIndex, true).c_str());
				break;
			}

			default:
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

			case ARROW_LEFT:
				if (MenuEntryList[CurrentIndex].EntryType == IntegerEntry)
				{
					(*MenuEntryList[CurrentIndex].Integer)--;

					ClearCurrentLine(TitleSize + CurrentIndex);
					wprintf(EntryString(CurrentIndex, true).c_str());
				}
				break;

			case ARROW_RIGHT:
				if (MenuEntryList[CurrentIndex].EntryType == IntegerEntry)
				{
					(*MenuEntryList[CurrentIndex].Integer)++;

					ClearCurrentLine(TitleSize + CurrentIndex);
					wprintf(EntryString(CurrentIndex, true).c_str());
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
			
			GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
			DWORD written, cells = csbi.dwSize.X * 2;
			FillConsoleOutputCharacter(ConsoleHandle, ' ', cells, tl, &written);
			FillConsoleOutputAttribute(ConsoleHandle, csbi.wAttributes, cells, tl, &written);

			/*
			What needs to be redrawing depending on if its up for down
			if the index goes down (bigger number), you need to clear above and current line
			
			|| Old Selected Entry
			\/ New Selected Entry <-- Here is Cursor

			and if going up (smaller number)
			/\ New Selected Entry <-- Here is Cursor
			|| Old Selected Entry
			*/

			COORD FinalPosition;

			if (CurrentIndex > OldIndex) /* Going Down */
			{

				SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)(TitleSize + CurrentIndex-1) });
				wprintf((EntryString(OldIndex, false) + EntryString(CurrentIndex, true)).c_str());

				if ((TitleSize + CurrentIndex) + (rows / 2) < 0)
					FinalPosition = { 0,0 };
				else if ((TitleSize + CurrentIndex) + (rows / 2) > MenuEntryList.ArrayIndexPointer)
					FinalPosition = { 0, (SHORT)(MenuEntryList.ArrayIndexPointer + TitleSize - 1) };
				else
					FinalPosition = { 0, (SHORT)((TitleSize + CurrentIndex) + (rows / 2)) };

				SetConsoleCursorPosition(ConsoleHandle, FinalPosition);
			}
			else /* Going Up */
			{
				SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)(TitleSize + CurrentIndex) });
				wprintf((EntryString(CurrentIndex, true) + EntryString(OldIndex, false)).c_str());

				if ((TitleSize + CurrentIndex) - (rows / 2) < 0)
					FinalPosition = { 0,0 };
				else if ((TitleSize + CurrentIndex) - (rows / 2) > MenuEntryList.ArrayIndexPointer)
					FinalPosition = { 0, (SHORT)MenuEntryList.ArrayIndexPointer };
				else
					FinalPosition = { 0, (SHORT)((TitleSize + CurrentIndex) - (rows / 2)) };

				SetConsoleCursorPosition(ConsoleHandle, FinalPosition);
			}

		}

		OldIndex = CurrentIndex;
	}
}

void DynamicMenu::DrawMenu(int CurrentIndex, int* TitleSize)
{
	clear_screen();

	GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	std::wstring OutputString; // string for full "display" as it is the most perfomace efficent method

	if (CustomTitle) /* If custom Title is true, its going to use the straight characters instead of generating a unicode title*/
		if (CenteredTitle)
			OutputString = Title;
		else
			OutputString = std::wstring(((columns / 2) - Title.length() / 2), ' ') + Title;
	else
		OutputString = AsciiTextGenerator::UnicodeTitleGenerate(Title, columns, CenteredTitle); // add title with "ascii generator"

	*TitleSize = 0;

	for (int i = 0; i < OutputString.size(); i++)
	{
		if (OutputString[i] == '\n')
		{
			(*TitleSize)++;
		}
	}

	// for loop using counter to get the index so to add the >< to the selected option
	for (int i = 0; i < MenuEntryList.ArrayIndexPointer; i++)
	{
		if (i == CurrentIndex)
			OutputString += EntryString(i, true);
		else
			OutputString += EntryString(i, false);
	}

	wprintf(OutputString.c_str());

	SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)(CurrentIndex) });
}

std::wstring DynamicMenu::EntryString(int EntryIndex, bool selected)
{
	int SpaceLenght = ((columns / 2) - MenuEntryList[EntryIndex].Name.length() / 2);
	std::wstring EntryText;

	switch (MenuEntryList[EntryIndex].EntryType) /* Different printing types for different entry types*/
	{
	case FunctionEntry:
		// Append to string as to make it be 1 print operation, makes it way quicker
		if (selected)
		{
			return std::wstring(SpaceLenght - 2, ' ') + L">>" + MenuEntryList[EntryIndex].Name + L"<<\n";
		}
		else
		{
			return std::wstring(SpaceLenght, ' ') + MenuEntryList[EntryIndex].Name + L"\n";
		}
		break;
	case SubMenuEntry:
		if (selected)
		{
			return std::wstring(SpaceLenght - 2, ' ') + L"\033[33m>>" + MenuEntryList[EntryIndex].Name + L"<<\033[0m\n";
		}
		else
		{
			return std::wstring(SpaceLenght, ' ') + L"\033[33m" + MenuEntryList[EntryIndex].Name + L"\033[0m\n";
		}
		break;

	case BooleanEntry:
		if (*MenuEntryList[EntryIndex].Boolean)
			EntryText = MenuEntryList[EntryIndex].Name + std::wstring(4, ' ') + L"[X]";
		else
			EntryText = MenuEntryList[EntryIndex].Name + std::wstring(4, ' ') + L"[ ]";

		if (selected)
		{
			return std::wstring(SpaceLenght - 2, ' ') + L">>" + EntryText + L"<<\n";
		}
		else
		{
			return std::wstring(SpaceLenght, ' ') + EntryText + L"\n";
		}
		break;

	case IntegerEntry:
		if (selected)
		{
			EntryText = MenuEntryList[EntryIndex].Name + std::wstring(4, ' ') + L"<" + std::to_wstring(*MenuEntryList[EntryIndex].Integer) + L">\n";
			return std::wstring(SpaceLenght - 2, ' ') + EntryText;
		}
		else
		{
			EntryText = MenuEntryList[EntryIndex].Name + std::wstring(4, ' ') + std::to_wstring(*MenuEntryList[EntryIndex].Integer);
			return std::wstring(SpaceLenght, ' ') + EntryText + L"\n";
		}
		break;

	case EmptyEntry:
		return std::wstring(SpaceLenght, ' ') + MenuEntryList[EntryIndex].Name + L"\n";
		break;
	}
}

void DynamicMenu::clear_screen(char fill)
{
	COORD tl = { 0,0 };
	GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
	DWORD written, cells = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(ConsoleHandle, fill, cells, tl, &written);
	FillConsoleOutputAttribute(ConsoleHandle, csbi.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(ConsoleHandle, tl);
}

void DynamicMenu::ClearCurrentLine(int Position)
{
	COORD tl = { 0, (SHORT)(Position) };
	GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
	DWORD written, cells = csbi.dwSize.X;
	FillConsoleOutputCharacter(ConsoleHandle, ' ', cells, tl, &written);
	FillConsoleOutputAttribute(ConsoleHandle, csbi.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(ConsoleHandle, tl);
}

void DynamicMenu::AddMenuEntry(MenuEntry Entry)
{
	MenuEntryList.Append(Entry);
}

void DynamicMenu::QuitMenu()
{
	ContinueMenu = false;
}