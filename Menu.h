#include <Windows.h>
#include "Game.h"

#pragma once

class Menu
{
private:
    HWND hwnd;
    HWND hwndImage;
    HMENU menu;
    HMENU menuFile;
    HMENU menuBack;
    HMENU menuGame;
    int sizeStr;
    OPENFILENAME openfile;
    WCHAR *nameFile;
    WNDCLASS wndClass;
    Game *game;
public:
    Menu(HWND hWnd);
    void Check(int ID);
    void Uncheck(int ID);
    void Enabled(int ID);
    void Disabled(int ID);
    void OpenImage(Game *gm);
    void ShowTip();
    void SetImage();
    bool IsChecked(int ID);
    WCHAR *GetImage();
    const static int ID_HELP = 10000;
    const static int ID_NUMBER = 10001;
    const static int ID_IMAGE = 10002;
    const static int ID_EXIT = 10003;
    const static int ID_TIP = 10004;
    const static int ID_NEWGAME = 10005;
    ~Menu(void);
};

