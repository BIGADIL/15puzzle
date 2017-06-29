#include <Windows.h>
#include "Form.h"

#pragma once

class FormImage : public Form
{
private:
    Gdiplus::Bitmap **cells;
    Gdiplus::SolidBrush *brushBack;
    int numberImage;
public:
    FormImage(HWND hwnd, int infPanelH, Gdiplus::Bitmap *bmp);
    void virtual DrawCells(Puzzle *puzzle, Shape *shape, int width, int height);
    void virtual DrawCell(RECT *background, RECT *cell, int number);
    void virtual DrawRightCombination(Shape *shape, int width, int height);
    virtual ~FormImage(void);
};

