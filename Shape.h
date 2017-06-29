#include <Windows.h>

#pragma once

class Shape
{
private:
    RECT **field;
    int border;
    int space;
    int numberX, numberY;
public:
    Shape(void);
    void Calculate(int width, int height, int infoPanelHeight);
    LPRECT GetCell(int x, int y);
    ~Shape(void);
};

