#include <Windows.h>
#include "Puzzle.h"
#include "Shape.h"
#include <gdiplus.h>

#pragma once
class Form
{
private:
    Gdiplus::SolidBrush *brushPanel;
    Gdiplus::SolidBrush *brushText;
protected:
    HWND hwnd;
    int infoPanelHeight;
public:
    Form(HWND hWnd, int infPanelH);
    void virtual DrawCells(Puzzle *puzzle, Shape *shape, int width, int height) = 0;
    void virtual DrawCell(RECT *background, RECT *cell, int number) = 0;
    void virtual DrawRightCombination(Shape *shape, int width, int height) = 0;
    void DrawInfoPanel(int width, WCHAR *counter, WCHAR *score);
    virtual ~Form(void);
};

