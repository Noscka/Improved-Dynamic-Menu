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
int number = 0;

void CheckBool()
{
    wprintf((SomeBool ? L"true" : L"false"));
    system("Pause");
}

void CheckNumber()
{
    wprintf(std::to_wstring(number).c_str());
    system("Pause");
}

int main()
{
    _setmode(_fileno(stdout), _O_U8TEXT);

    DynamicMenu MainMenu = DynamicMenu(L"Main Menu", false, true, true);

    DynamicMenu SecondaryMenu = DynamicMenu(L"Second Menu", false, true, true);

    MainMenu.AddMenuEntry(MenuEntry(L"Another Menu", &SecondaryMenu));

    MainMenu.AddMenuEntry(MenuEntry(L"Toggle", &SomeBool));
    MainMenu.AddMenuEntry(MenuEntry(L"Check Bool", CheckBool));

    MainMenu.AddMenuEntry(MenuEntry(L"Number", &number));
    MainMenu.AddMenuEntry(MenuEntry(L"Check Number", CheckNumber));

    for (int i = 0; i < 30; i++)
    {
        MainMenu.AddMenuEntry(MenuEntry(std::to_wstring(i), Print));
    }

    MainMenu.StartMenu();
}
