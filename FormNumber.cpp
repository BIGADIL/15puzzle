#define _CRT_SECURE_NO_WARNINGS


#include "FormNumber.h"

using namespace Gdiplus;

FormNumber::FormNumber(HWND hWnd, int infPanelH) : Form(hWnd, infPanelH)
{
    brushBack = new SolidBrush(Color(220,220,220));
    brushText = new SolidBrush(Color(255,255,255));
    brushCell = new SolidBrush(Color(180,180,180));
    fontFamily = new FontFamily(L"Arial");
    border = 4;
    penBorder = new Pen(Color(140,140,140),border);
}

void FormNumber::DrawCells(Puzzle *puzzle, Shape *shape, int width, int height)
{
    HDC hdc = GetDC(hwnd);
    Graphics gr(hdc);
    Bitmap backBuffer(width,height,&gr);
    Graphics temp(&backBuffer);
    LPRECT rect = shape->GetCell(0,0);
    Font font(fontFamily, rect->bottom / 2);
    int border = 4;
    WCHAR str[3];
    height -= infoPanelHeight;
    temp.FillRectangle(brushBack,0,infoPanelHeight,width,height);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            int number = puzzle->GetCell(i,j);
            if(number != 16)
            {
                rect = shape->GetCell(i,j);
                temp.FillRectangle(brushCell,rect->left,rect->top,rect->right,rect->bottom);
                _itow(number,str,10);
                temp.DrawRectangle(penBorder,rect->left + border / 2,rect->top + border / 2,rect->right - border,rect->bottom - border);
                temp.DrawString(str,-1,&font,PointF(rect->left + border,rect->top + border),brushText);
            }
        }
    }
    gr.DrawImage(&backBuffer,0,0);
    ReleaseDC(hwnd, hdc);
}

void FormNumber::DrawCell(RECT *background, RECT *cell, int number)
{
    HDC hdc = GetDC(hwnd);
    Graphics gr(hdc);
    Font font(fontFamily, cell->bottom / 2);
    WCHAR str[3];
    gr.FillRectangle(brushBack,background->left,background->top,background->right,background->bottom);
    gr.FillRectangle(brushCell,cell->left,cell->top,cell->right,cell->bottom);
    _itow(number,str,10);
    gr.DrawRectangle(penBorder,cell->left + border / 2,cell->top + border / 2,cell->right - border,cell->bottom - border);
    gr.DrawString(str,-1,&font,PointF(cell->left + border,cell->top + border),brushText);
    ReleaseDC(hwnd,hdc);
}

void FormNumber::DrawRightCombination(Shape *shape, int width, int height)
{
    HDC hdc = GetDC(hwnd);
    Graphics gr(hdc);
    Bitmap backBuffer(width,height,&gr);
    Graphics temp(&backBuffer);
    LPRECT rect = shape->GetCell(0,0);
    Font font(fontFamily, rect->bottom / 2);
    int border = 4;
    WCHAR str[3];
    height -= infoPanelHeight;
    temp.FillRectangle(brushBack,0,infoPanelHeight,width,height);
    int number = 1;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            rect = shape->GetCell(i,j);
            temp.FillRectangle(brushCell,rect->left,rect->top,rect->right,rect->bottom);
            _itow(number,str,10);
            temp.DrawRectangle(penBorder,rect->left + border / 2,rect->top + border / 2,rect->right - border,rect->bottom - border);
            temp.DrawString(str,-1,&font,PointF(rect->left + border,rect->top + border),brushText);
            number++;
        }
    }
    gr.DrawImage(&backBuffer,0,0);
    ReleaseDC(hwnd, hdc);
}

FormNumber::~FormNumber(void)
{
    delete brushBack;
    delete brushCell;
    delete brushText;
    delete fontFamily;
    delete penBorder;
}
