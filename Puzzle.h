#include <Windows.h>

#pragma once

class Puzzle
{
private:
    int **field;
    int width, height;
    int empty;
    POINT emptyCell;
    POINT lastMoved;
    void MixCells();
    bool IsSolvable(int *mas, int len);
public:
    Puzzle(void);
    void StartNewGame();
    bool Left();
    bool Right();
    bool Up();
    bool Down();
    int GetCell(int x, int y);
    bool IsGameEnd();
    POINT GetEmpty();
    POINT GetLastMoved();
    ~Puzzle(void);
};

