#include <iostream>
#include "DynamicMenu/DynamicMenu.h"
#include <io.h>
#include <fcntl.h>

void Print()
{
    wprintf(L"gwagawga");
    system("Pause");
}

void up()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { 3, 6 };
    SetConsoleCursorPosition(hConsole, pos);
    system("Pause");
}

int main()
{
    bool SomeBool;

    _setmode(_fileno(stdout), _O_U8TEXT);

    DynamicMenu SomeMenu = DynamicMenu(L"gaga", false, true);

    MenuEntry ME = MenuEntry(L"oogae", Print);
    ME.EntryType = SubMenuEntry;

    SomeMenu.AddMenuEntry(ME);
    SomeMenu.AddMenuEntry(MenuEntry(L"OnOff", &SomeBool));



    for (int i = 0; i < 30; i++)
    {
        SomeMenu.AddMenuEntry(MenuEntry(L"gawg", Print));
    }

    SomeMenu.AddMenuEntry(MenuEntry(L"Up", up));

    SomeMenu.StartMenu();
}
