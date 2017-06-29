#include "Menu.h"
#include <gdiplus.h>

using namespace Gdiplus;

LRESULT CALLBACK wndImageProc(HWND, UINT, WPARAM, LPARAM);

void OnPaint(HDC, RECT*, RECT*, bool);

void SizingRegion(RECT *rectImage, RECT *rectRegion, POINT *pointLeft, POINT *pointRight);

void AdaptRectImage(RECT *rect);

static Menu *menuObject;
static int imageWndWidth = 500;
static int imageWndHeight = 500;
static double factor;

static Bitmap *image;
static Bitmap *selectedImage;

Menu::Menu(HWND hWnd)
{
    hwnd = hWnd;
    menu = CreateMenu();
    menuFile = CreatePopupMenu();
    menuBack = CreatePopupMenu();
    menuGame = CreatePopupMenu();
    AppendMenu(menu,MF_STRING | MF_POPUP,(UINT_PTR)menuFile,TEXT("Файл"));
    AppendMenu(menuFile,MF_STRING,ID_NEWGAME,"Новая игра");
    AppendMenu(menuFile,MF_STRING | MF_POPUP,(UINT_PTR)menuBack,TEXT("Сменить фон"));
    AppendMenu(menuFile,MF_SEPARATOR,0,NULL);
	AppendMenu(menuFile, MF_STRING, ID_EXIT, TEXT("Выход"));
	AppendMenu(menuBack, MF_STRING, ID_NUMBER, TEXT("Стандартный фон"));
	AppendMenu(menuBack, MF_STRING, ID_IMAGE, TEXT("Изображение"));
    AppendMenu(menu,MF_STRING | MF_POPUP,(UINT_PTR)menuGame,"Игра");
    AppendMenu(menuGame,MF_STRING,ID_TIP,"Итоговое изображение");
    AppendMenu(menu,MF_STRING,ID_HELP,"Помощь");
    SetMenu(hwnd,menu);

    Disabled(ID_TIP);
    Check(ID_NUMBER);
    sizeStr = 500;
    nameFile = new WCHAR[sizeStr];
    nameFile[0]=0;

    openfile.lStructSize = sizeof(OPENFILENAME);
    openfile.hwndOwner = hwnd;
    openfile.lpstrFile = (LPSTR)nameFile;
    openfile.nMaxFile = sizeStr;
    openfile.lpstrInitialDir = ".\\";
    openfile.lpstrTitle = "Открыть изображение";
    openfile.lpstrFilter = "*Изображения\0.bmp;*.png;*.jpg;*.gif\0\0";

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = wndImageProc;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.lpszMenuName = NULL;
    wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
    wndClass.lpszClassName = TEXT("WndImage");
    wndClass.hbrBackground = (HBRUSH)NULL_BRUSH;
    RegisterClass(&wndClass);

    menuObject = this;
}

void Menu::Check(int ID)
{
    CheckMenuItem(menu,ID,MF_CHECKED);
}

void Menu::Uncheck(int ID)
{
    CheckMenuItem(menu,ID,MF_UNCHECKED);
}

void Menu::Enabled(int ID)
{
    EnableMenuItem(menu,ID,MF_ENABLED);
}

void Menu::Disabled(int ID)
{
    EnableMenuItem(menu,ID,MF_GRAYED);
}

void Menu::OpenImage(Game *gm)
{
    if(GetOpenFileName(&openfile))
    {
        game = gm;
        image = new Bitmap(GetImage());
        hwndImage = CreateWindow("WndImage","Изображение",WS_OVERLAPPED | WS_CAPTION | WS_VISIBLE, CW_USEDEFAULT,
            CW_USEDEFAULT,imageWndWidth,imageWndHeight,hwnd,NULL,NULL,NULL);
    }
}

void Menu::SetImage()
{
    game->SetBackgroundImage(selectedImage);
    Enabled(ID_TIP);
}

bool Menu::IsChecked(int ID)
{
    int fl = GetMenuState(menu,ID,MF_BYCOMMAND);
    if(!(fl & MF_CHECKED))
    {
        return false;
    }
    else
    {
        return true;
    }
}

WCHAR *Menu::GetImage()
{
    return nameFile;
}

Menu::~Menu(void)
{
    DestroyMenu(menu);
    delete nameFile;
}

LRESULT CALLBACK wndImageProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static RECT rectClient;
    static HWND buttonOK;
    static HWND buttonCancel;
    static HWND checkButtonStretch;
    static HWND checkButtonSquare;
    const int ID_buttonOK = 1001;
    const int ID_buttonCancel = 1002;
    const int ID_checkBtnSquare = 1003;
    const int ID_checkBtnStretch = 1004;
    static RECT rectRegionImage;
    static RECT rectImage;
    static bool captureMouse;
    static POINT points[2];
    static int number;
    static bool drawingRegion;
    int X, Y, difX, difY;
    switch(message)
    {
    case WM_CREATE:
        GetClientRect(hwnd, &rectClient);
        buttonOK = CreateWindow("BUTTON", "OK", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, rectClient.right - 340,
            rectClient.bottom - 50, 150, 30, hwnd, (HMENU)ID_buttonOK, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
        buttonCancel = CreateWindow("BUTTON", "Отмена", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, rectClient.right - 170,
            rectClient.bottom - 50, 150, 30, hwnd, (HMENU)ID_buttonCancel, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
        checkButtonStretch = CreateWindow("BUTTON", "Растянуть изображение", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BST_CHECKED, 
            rectClient.left + 20, rectClient.top + 40, 400, 30, hwnd, (HMENU)ID_checkBtnStretch, 
            (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
        checkButtonSquare = CreateWindow("BUTTON", "Выбрать регион", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BST_CHECKED, 
            rectClient.left + 20, rectClient.top + 70, 400, 30, hwnd, (HMENU)ID_checkBtnSquare, 
            (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
        SendMessage(checkButtonStretch,BM_CLICK,0,0);
        drawingRegion = false;
        rectImage.left = rectClient.left + 20;
        rectImage.top = rectClient.top + 100;
        rectImage.right = rectClient.right - 40;
        rectImage.bottom = rectClient.bottom - 170;
        AdaptRectImage(&rectImage);
        points[0].x = rectRegionImage.left = rectImage.left + 25;
        points[0].y = rectRegionImage.top = rectImage.top + 15;
        rectRegionImage.right = rectImage.right - 50;
        rectRegionImage.bottom = rectImage.bottom - 30;
        points[1].x = rectRegionImage.right + rectRegionImage.left;
        points[1].y = rectRegionImage.bottom + rectRegionImage.top;
        break;
    case WM_LBUTTONDOWN:
        X = LOWORD(lparam);
        Y = HIWORD(lparam);
        for(number = 0; number < 2; ++number)
        {
            difX = X - points[number].x;
            difY = Y - points[number].y;
            if(difX * difX + difY * difY <= 100)
            {
                SetCapture(hwnd);
                captureMouse = true;
                break;
            }
        }
        break;
    case WM_MOUSEMOVE:
        if(captureMouse)
        {
            points[number].x = LOWORD(lparam);
            points[number].y = HIWORD(lparam);
            if(!number)
            {
                rectRegionImage.left = points[number].x;
                rectRegionImage.top = points[number].y;
                points[number + 1].x = rectRegionImage.left + rectRegionImage.right;
                points[number + 1].y = rectRegionImage.top + rectRegionImage.bottom;
            }
            else
            {
                rectRegionImage.right = points[number].x - rectRegionImage.left;
                rectRegionImage.bottom = points[number].y - rectRegionImage.top;
            }
            SizingRegion(&rectImage,&rectRegionImage,&points[0],&points[1]);
            hdc = GetDC(hwnd);
            OnPaint(hdc,&rectImage,&rectRegionImage,drawingRegion);
            ReleaseDC(hwnd,hdc);
        }
        break;
    case WM_LBUTTONUP:
        if(captureMouse)
        {
            ReleaseCapture();
            captureMouse = false;
        }
        break;
    case WM_COMMAND:
        switch(wparam)
        {
        case ID_buttonOK:
            if(SendMessage(checkButtonStretch,BM_GETCHECK,0,0) == BST_CHECKED)
            {
                selectedImage = image->Clone(0,0,image->GetWidth(),image->GetHeight(),PixelFormat());
            }
            if(SendMessage(checkButtonSquare,BM_GETCHECK,0,0) == BST_CHECKED)
            {
                selectedImage = image->Clone(REAL((rectRegionImage.left - rectImage.left) / factor), 
                    REAL((rectRegionImage.top - rectImage.top) / factor),
                    REAL(rectRegionImage.right / factor), REAL(rectRegionImage.bottom / factor),PixelFormat());
            }
            menuObject->SetImage();
            menuObject->Check(menuObject->ID_IMAGE);
            menuObject->Uncheck(menuObject->ID_NUMBER);
            PostMessage(hwnd,WM_CLOSE,0,0);
            break;
        case ID_buttonCancel:
            PostMessage(hwnd,WM_CLOSE,0,0);
            break;
        case ID_checkBtnSquare:
            SendMessage(checkButtonStretch,BM_SETCHECK,0,0);
            drawingRegion = true;
            hdc = GetDC(hwnd);
            OnPaint(hdc,&rectImage,&rectRegionImage,drawingRegion);
            ReleaseDC(hwnd,hdc);
            break;
        case ID_checkBtnStretch:
            SendMessage(checkButtonSquare,BM_SETCHECK,0,0);
            drawingRegion = false;
            hdc = GetDC(hwnd);
            OnPaint(hdc,&rectImage,&rectRegionImage,drawingRegion);
            ReleaseDC(hwnd,hdc);
            break;
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd,&ps);
        OnPaint(hdc, &rectImage,&rectRegionImage,drawingRegion);
        EndPaint(hwnd,&ps);
        break;
    case WM_CLOSE:
        delete image;
        DestroyWindow(hwnd);
        break;
    default:
        return DefWindowProc(hwnd, message, wparam, lparam);
    }
    return 0;
}

void OnPaint(HDC hdc, RECT *rectImage, RECT *rectRegion, bool region)
{
    Graphics gr(hdc);
    SolidBrush brushText(Color(40,40,40));
    Font font(L"Arial", 12);
    gr.DrawString(L"Вы можете выбрать изображение целиком, либо его регион.",-1,&font,PointF(10,10),&brushText);
    gr.DrawImage(image, rectImage->left, rectImage->top, rectImage->right, rectImage->bottom);
    if(region)
    {
        Pen pen(Color(175,175,175),2);
        gr.DrawRectangle(&pen,rectRegion->left,rectRegion->top,rectRegion->right,rectRegion->bottom);
        SolidBrush brushPoint(Color(50,200,50));
        gr.FillRectangle(&brushPoint,rectRegion->left,rectRegion->top, 6, 6);
        gr.FillRectangle(&brushPoint,rectRegion->left + rectRegion->right - 6,rectRegion->top + rectRegion->bottom - 6, 6, 6);
    }
}

void SizingRegion(RECT *rectImage, RECT *rectRegion, POINT *pointLeft, POINT *pointRight)
{
    if(rectImage->left > pointLeft->x)
    {
        rectRegion->left = pointLeft->x = rectImage->left;
    }
    if(rectImage->top > pointLeft->y)
    {
        rectRegion->top = pointLeft->y = rectImage->top;
    }
    if(rectImage->left + rectImage->right < pointRight->x)
    {
        pointRight->x = rectImage->left + rectImage->right;
        rectRegion->right = pointRight->x - rectRegion->left;
    }
    if(rectImage->top + rectImage->bottom < pointRight->y)
    {
        pointRight->y = rectImage->top + rectImage->bottom;
        rectRegion->bottom = pointRight->y - rectRegion->top;
    }
}

void AdaptRectImage(RECT *rect)
{
    double factorX = 0, factorY = 0, imgW = 0, imgH = 0;
    imgW = image->GetWidth();
    imgH = image->GetHeight();
    factorX = rect->right / imgW;
    factorY = rect->bottom / imgH;
    if(factorX > factorY)
    {
        
        int regionWidth = imgW * factorY;
        rect->left += (rect->right - regionWidth) / 2;
        rect->right = regionWidth;
        factor = factorY;
    }
    else
    {
        int regionHeight = imgH * factorX;
        rect->top += (rect->bottom - regionHeight) / 2;
        rect->bottom = regionHeight;
        factor = factorX;
    }
}