#include "windows.h"
#include "objidl.h"
#include "gdiplus.h"
#include "Game.h"
#include "Menu.h"

using namespace Gdiplus;

#pragma comment (lib, "Gdiplus.lib")

void OnPaint(HDC hdc);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND hwnd;
    MSG msg;
    WNDCLASS wndClass;
    GdiplusStartupInput gdiStartup;
    ULONG_PTR gdiToken;

    GdiplusStartup(&gdiToken, &gdiStartup, NULL);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.lpszMenuName = NULL;
    wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
    wndClass.lpszClassName = TEXT("Puzzle");
    wndClass.hbrBackground = NULL;

    RegisterClass(&wndClass);

    hwnd = CreateWindow(TEXT("Puzzle"), TEXT("Пятнашки"), WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
        300 + GetSystemMetrics(SM_CXBORDER) * 2, 300 + GetSystemMetrics(SM_CYBORDER) + GetSystemMetrics(SM_CYCAPTION) +
        GetSystemMetrics(SM_CYMENU) + 30, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    GdiplusShutdown(gdiToken);
    return 0;
}
void paint(HDC hdc);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static Game *game = new Game(hwnd,300,300);
    static Menu menu(hwnd);
    switch(message)
    {
    case WM_SIZE:
        game->Resize(LOWORD(lparam),HIWORD(lparam));
        break;
    case WM_SIZING:
        LPRECT rect;
        rect=(LPRECT)lparam;
        switch(wparam)
        {
        case WMSZ_TOP:
        case WMSZ_TOPRIGHT:
        case WMSZ_BOTTOM:
            rect->right = rect->bottom - rect->top + rect->left + GetSystemMetrics(SM_CXBORDER) * 2 - game->GetInfoPanelHeight() -
                GetSystemMetrics(SM_CYBORDER) - GetSystemMetrics(SM_CYMENU) - GetSystemMetrics(SM_CYCAPTION);
            break;
        case WMSZ_BOTTOMRIGHT:
        case WMSZ_RIGHT:
        case WMSZ_LEFT:
        case WMSZ_BOTTOMLEFT:
            rect->bottom = rect->right - rect->left + rect->top + GetSystemMetrics(SM_CYBORDER) + game->GetInfoPanelHeight() +
                GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CXBORDER) * 2;
            break;
        case WMSZ_TOPLEFT:
            rect->left = rect->right - rect->bottom + rect->top - GetSystemMetrics(SM_CXBORDER) * 2 + game->GetInfoPanelHeight() +
                GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYBORDER);
            break;
        }
    case WM_TIMER:
        switch(wparam)
        {
        case /*game->ID_TimerAnimation*/1001:
            game->Animation();
            break;
        case /*game->ID_TimerClock*/1002:
            game->InfoPanel();
            break;
        }
        break;
    case WM_LBUTTONDOWN:
        game->MouseClick(LOWORD(lparam), HIWORD(lparam));
        break;
    case WM_KEYDOWN:
        switch (wparam)
        {
        case VK_LEFT:
            game->Left();
            break;
        case VK_RIGHT:
            game->Right();
            break;
        case VK_UP:
            game->Up();
            break;
        case VK_DOWN:
            game->Down();
            break;
        }
        if(menu.IsChecked(menu.ID_TIP))
        {
            menu.Uncheck(menu.ID_TIP);
        }
        break;
    case WM_COMMAND:
        switch(wparam)
        {
        case menu.ID_HELP:
            MessageBox(hwnd,TEXT("Создатель - Лаврик А."),TEXT("HELP"),MB_OK);
            break;
        case menu.ID_EXIT:
            PostQuitMessage(0);
            break;
        case menu.ID_NEWGAME:
            game->Restart();
            break;
        case menu.ID_NUMBER:
            game->SetBackgroundNumber();
            menu.Check(menu.ID_NUMBER);
            menu.Uncheck(menu.ID_IMAGE);
            menu.Disabled(menu.ID_TIP);
            break;
        case menu.ID_IMAGE:
            menu.OpenImage(game);
            break;
        case menu.ID_TIP:
            game->ShowRightCombination();
            if(menu.IsChecked(menu.ID_TIP))
            {
                menu.Uncheck(menu.ID_TIP);
            }
            else
            {
                menu.Check(menu.ID_TIP);
            }
            break;
        }
        break;
    case WM_PAINT:
        BeginPaint(hwnd,&ps);
        game->Draw();
        EndPaint(hwnd,&ps);
        break;
    case WM_DESTROY:
        delete game;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wparam, lparam);
    }
    return 0;
}