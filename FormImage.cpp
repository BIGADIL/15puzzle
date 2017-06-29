#include "FormImage.h"

using namespace Gdiplus;

FormImage::FormImage(HWND hwnd, int infPanelH, Bitmap *bmp) : Form(hwnd, infPanelH)
{
    numberImage = 16;
    cells = new Bitmap*[numberImage];
    int number = 0;
    for(int x = 0; x < 4; x++)
    {
        for(int y = 0; y < 4; y++)
        {
            cells[number] = bmp->Clone(REAL(bmp->GetWidth() / 4 * y), REAL(bmp->GetHeight() / 4 * x),
                REAL(bmp->GetWidth() / 4), REAL(bmp->GetHeight() / 4), PixelFormat());
            number++;
        }
    }
    brushBack = new SolidBrush(Color(150,150,150));
    delete bmp;
}

void FormImage::DrawCells(Puzzle *puzzle, Shape *shape, int width, int height)
{
    HDC hdc = GetDC(hwnd);
    Graphics gr(hdc);
    Bitmap backBuffer(width,height,&gr);
    Graphics temp(&backBuffer);
    height -= infoPanelHeight;
    temp.FillRectangle(brushBack,0,infoPanelHeight,width,height);
    for(int x = 0; x < 4; x++)
    {
        for(int y = 0; y < 4; y++)
        {
            int number = puzzle->GetCell(x,y);
            if(number != 16)
            {
                LPRECT rect = shape->GetCell(x,y);
                temp.DrawImage(cells[number - 1],rect->left,rect->top,rect->right,rect->bottom);
            }
        }
    }
    gr.DrawImage(&backBuffer,0,0);
    ReleaseDC(hwnd, hdc);
}

void FormImage::DrawCell(RECT *background, RECT *cell, int number)
{
    HDC hdc = GetDC(hwnd);
    Graphics gr(hdc);
    gr.FillRectangle(brushBack,background->left,background->top,background->right,background->bottom);
    gr.DrawImage(cells[number - 1],cell->left,cell->top,cell->right,cell->bottom);
    ReleaseDC(hwnd,hdc);
}

void FormImage::DrawRightCombination(Shape *shape, int width, int height)
{
    HDC hdc = GetDC(hwnd);
    Graphics gr(hdc);
    Bitmap backBuffer(width,height,&gr);
    Graphics temp(&backBuffer);
    height -= infoPanelHeight;
    temp.FillRectangle(brushBack,0,infoPanelHeight,width,height);
    int number = 0;
    for(int x = 0; x < 4; x++)
    {
        for(int y = 0; y < 4; y++)
        {
            LPRECT rect = shape->GetCell(x,y);
            temp.DrawImage(cells[number],rect->left,rect->top,rect->right,rect->bottom);
            number++;
        }
    }
    gr.DrawImage(&backBuffer,0,0);
    ReleaseDC(hwnd, hdc);
}

FormImage::~FormImage(void)
{
    for(int i = 0; i < numberImage; i++)
    {
        delete cells[i];
    }
    delete[] cells;
    delete brushBack;
}
