#include <iostream>
#include "DynamicMenu/DynamicMenu.h"
#include <io.h>
#include <fcntl.h>

void Print()
{
    wprintf(L"gwagawga");
    system("Pause");
}

int main()
{
    _setmode(_fileno(stdout), _O_U8TEXT);

    DynamicMenu SomeMenu = DynamicMenu(L"gaga", false, true);

    DynamicMenu secondmenu = DynamicMenu(L"Menu2", false, true);
    secondmenu.AddMenuEntry(MenuEntry(L"some", Print));
    secondmenu.AddMenuEntry(MenuEntry(L"some", Print));


    SomeMenu.AddMenuEntry(MenuEntry(L"some", Print));
    SomeMenu.AddMenuEntry(MenuEntry(L"some2", &secondmenu));
    SomeMenu.AddMenuEntry(MenuEntry(L"2", Print));
    SomeMenu.CreateMenu();
}
