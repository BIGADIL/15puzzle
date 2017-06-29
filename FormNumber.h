#include <Windows.h>
#include "form.h"

#pragma once

class FormNumber :
    public Form
{
private:
    Gdiplus::SolidBrush *brushBack;
    Gdiplus::SolidBrush *brushCell;
    Gdiplus::SolidBrush *brushText;
    Gdiplus::FontFamily *fontFamily;
    Gdiplus::Pen *penBorder;
    int border;
public:
    FormNumber(HWND hWnd, int infPanelH);
    void virtual DrawCells(Puzzle *puzzle, Shape *shape, int width, int height);
    void virtual DrawCell(RECT *background, RECT *cell, int number);
    void virtual DrawRightCombination(Shape *shape, int width, int height);
    virtual ~FormNumber(void);
};

