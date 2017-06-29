#include "Shape.h"


Shape::Shape(void)
{
    numberX = numberY = 4;
    field = new RECT*[numberX];
    for(int i = 0; i < numberX; i++)
    {
        field[i] = new RECT[numberY];
    }
    border = 3;
    space = 1;
    Calculate(300, 300, 15);
}

void Shape::Calculate(int width, int height, int infoPanelHeight)
{
    int cellWidth = 0, cellHeight = 0;
    cellWidth = (width - border * 2 - (numberX * space -1)) / numberX;
    cellHeight = (height - infoPanelHeight - border * 2 - (numberY * space - 1)) / numberY;
    for(int x = 0; x < numberX; x++)
    {
        for(int y = 0; y < numberY; y++)
        {
            field[x][y].left = border + (y * cellWidth + y * space);
            field[x][y].top = border + (x * cellHeight + x * space) + infoPanelHeight;
            field[x][y].bottom = cellHeight;
            field[x][y].right = cellWidth;
        }
    }
}

LPRECT Shape::GetCell(int x, int y)
{
    if(x < numberX && y < numberY)
    {
        return &field[x][y];
    }
    else
    {
        return NULL;
    }
}

Shape::~Shape(void)
{
    for(int i = 0; i < numberX; i++)
    {
        delete[] field[i];
    }
    delete[] field;
}
