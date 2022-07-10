#include <iostream>
#include "DynamicMenu/DynamicMenu.h"
#include <io.h>
#include <fcntl.h>

void Print()
{
    wprintf(L"gwagawga");
    system("Pause");
}

bool SomeBool = false;

void CheckBool()
{
    wprintf(SomeBool ? L"true" : L"false");
    system("Pause");
}

int main()
{
    _setmode(_fileno(stdout), _O_U8TEXT);

    DynamicMenu SomeMenu = DynamicMenu(L"gaga", false, true);

    MenuEntry ME = MenuEntry(L"oogae", Print);
    ME.EntryType = SubMenuEntry;

    SomeMenu.AddMenuEntry(ME);
    SomeMenu.AddMenuEntry(MenuEntry(L"OnOff", &SomeBool));

    SomeMenu.AddMenuEntry(MenuEntry(L"Check Bool", CheckBool));

    for (int i = 0; i < 30; i++)
    {
        SomeMenu.AddMenuEntry(MenuEntry(L"gawg", Print));
    }

    SomeMenu.StartMenu();
}
