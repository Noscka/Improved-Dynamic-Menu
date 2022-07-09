#include <iostream>
#include "DynamicMenu/DynamicMenu.h"
#include <io.h>
#include <fcntl.h>

void Print()
{
    wprintf(L"gwagawga");
}

int main()
{
    _setmode(_fileno(stdout), _O_U8TEXT);

    DynamicMenu SomeMenu = DynamicMenu();

    DynamicMenu secondmenu = DynamicMenu();

    SomeMenu.AddMenuEntry(MenuEntry(L"some", Print));
    SomeMenu.AddMenuEntry(MenuEntry(L"some", &secondmenu));
    SomeMenu.AddMenuEntry(MenuEntry(L"2", Print));
    SomeMenu.CreateMenu(L"gaga");
}
