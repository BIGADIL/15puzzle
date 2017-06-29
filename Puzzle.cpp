#include "Puzzle.h"
#include <cstdio>
#include <ctime>

Puzzle::Puzzle(void)
{
    width = height = 4;
    field = new int*[width];
    for(int i = 0; i < width; i++)
    {
        field[i] = new int[height];
    }
    StartNewGame();
    empty = 16;
    lastMoved.x = 0;
    lastMoved.y = 0;
}

void Puzzle::StartNewGame()
{
    MixCells();
    emptyCell.x = 3;
    emptyCell.y = 3;
}

bool Puzzle::Down()
{
    if(emptyCell.x > 0)
    {
        lastMoved = emptyCell;
        field[emptyCell.x][emptyCell.y] = field[emptyCell.x - 1][emptyCell.y];
        emptyCell.x -= 1;
        field[emptyCell.x][emptyCell.y] = empty;
        return true;
    }
    else
    {
        return false;
    }
}

bool Puzzle::Up()
{
    if(emptyCell.x < width - 1)
    {
        lastMoved = emptyCell;
        field[emptyCell.x][emptyCell.y] = field[emptyCell.x + 1][emptyCell.y];
        emptyCell.x += 1;
        field[emptyCell.x][emptyCell.y] = empty;
        return true;
    }
    else
    {
        return false;
    }
}

bool Puzzle::Right()
{
    if(emptyCell.y > 0)
    {
        lastMoved = emptyCell;
        field[emptyCell.x][emptyCell.y] = field[emptyCell.x][emptyCell.y - 1];
        emptyCell.y -= 1;
        field[emptyCell.x][emptyCell.y] = empty;
        return true;
    }
    else
    {
        return false;
    }
}

bool Puzzle::Left()
{
    if(emptyCell.y < height - 1)
    {
        lastMoved = emptyCell;
        field[emptyCell.x][emptyCell.y] = field[emptyCell.x][emptyCell.y + 1];
        emptyCell.y += 1;
        field[emptyCell.x][emptyCell.y] = empty;
        return true;
    }
    else
    {
        return false;
    }
}

int Puzzle::GetCell(int x, int y)
{
    if(x < width && y < height)
    {
        return field[x][y];
    }
    else
    {
        return -1;
    }
}

bool Puzzle::IsGameEnd()
{
    int number = 1;
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            if(field[x][y] != number)
            {
                return false;
            }
            number++;
        }
    }
    return true;
}

POINT Puzzle::GetEmpty()
{
    return emptyCell;
}

POINT Puzzle::GetLastMoved()
{
    return lastMoved;
}

void Puzzle::MixCells()
{
    int length = width * height;
    int *mix = new int[length];
    for(int i = 0;i < length; i++)
    {
        mix[i] = i + 1;
    }
    srand(time(0));
    do
    {
        for(int i = 0; i < length - 1; i++)
        {
            int number = rand() % (length - 1);
            int temp = mix[i];
            mix[i] = mix[number];
            mix[number] = temp;
        }
    }
    while(!IsSolvable(mix, length));
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            field[x][y] = mix[x * width + y];
        }
    }
    delete[] mix;
}

bool Puzzle::IsSolvable(int *mas, int len)
{
    int sum = 0;
    int emptyCooY = 4;
    for(int i = 0; i < len - 1; i++)
    {
        for(int j = i + 1; j < len - 1; j++)
        {
            if(mas[i] > mas[j])
            {
                sum ++;
            }
        }
    }
    sum += emptyCooY;
    if(sum % 2 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Puzzle::~Puzzle(void)
{
    for(int i = 0; i < width; i++)
    {
        delete[] field[i];
    }
    delete[] field;
}
