#define _CRT_SECURE_NO_WARNINGS

#include "Game.h"


Game::Game(HWND hWnd, int width, int height)
{
    hwnd = hWnd;
    infoPanelHeight = 30;
    puzzle = new Puzzle();
    shape = new Shape();
    form = new FormNumber(hWnd,infoPanelHeight);
    shape->Calculate(width, height,infoPanelHeight);
    _width = width;
    _height = height;
    countAnimation = 0;
    lengthAnimation = 12;
    state = GameStateActive;
    score = 0;
    counterStart = GetTickCount();
    _itow(0,strTime,10);
    _itow(score,strScore,10);
    SetTimer(hwnd,ID_TimerClock,1000,NULL);
}

void Game::Left()
{
    if(state == GameStateActive)
    {
        if(puzzle->Left())
        {
            Animation();
            state = GameStateAnimation;
            score++;
            Score();
            form->DrawInfoPanel(_width,strTime,strScore);
        }
    }
    if(state == GameStatePause)
    {
        state = GameStateActive;
        Draw();
    }
}

void Game::Right()
{
    if(state == GameStateActive)
    {
        if(puzzle->Right())
        {
            Animation();
            state = GameStateAnimation;
            score++;
            Score();
            form->DrawInfoPanel(_width,strTime,strScore);
        }
    }
    if(state == GameStatePause)
    {
        state = GameStateActive;
        Draw();
    }
}

void Game::Up()
{
    if(state == GameStateActive)
    {
        if(puzzle->Up())
        {
            Animation();
            state = GameStateAnimation;
            score++;
            Score();
            form->DrawInfoPanel(_width,strTime,strScore);
        }
    }
    if(state == GameStatePause)
    {
        state = GameStateActive;
        Draw();
    }
}

void Game::Down()
{
    if(state == GameStateActive)
    {
        if(puzzle->Down())
        {
            Animation();
            state = GameStateAnimation;
            score++;
            Score();
            form->DrawInfoPanel(_width,strTime,strScore);
        }
    }
    if(state == GameStatePause)
    {
        state = GameStateActive;
        Draw();
    }
}

void Game::MouseClick(int x, int y)
{
    POINT point = puzzle->GetEmpty();
    if(IsInRegion(point.x + 1, point.y, x, y))
    {
        Up();
    }
    else if(IsInRegion(point.x, point.y + 1, x, y))
    {
        Left();
    }
    else if(IsInRegion(point.x - 1, point.y, x, y))
    {
        Down();
    }
    else if(IsInRegion(point.x, point.y - 1, x, y))
    {
        Right();
    }
}

void Game::Draw()
{
    switch(state)
    {
    case GameStateActive:
        form->DrawInfoPanel(_width,strTime,strScore);
        form->DrawCells(puzzle,shape,_width,_height);
        break;
    case GameStatePause:
        form->DrawInfoPanel(_width,strTime,strScore);
        form->DrawRightCombination(shape,_width,_height);
        break;
    case GameStateEnd:
        form->DrawRightCombination(shape,_width,_height);
        form->DrawInfoPanel(_width,strTime,strScore);
    }
}

void Game::Animation()
{
    int number;
    POINT pointEmpty = puzzle->GetEmpty();
    POINT pointCell = puzzle->GetLastMoved();
    number = puzzle->GetCell(pointCell.x,pointCell.y);
    if(countAnimation == 0)
    {
        SetTimer(hwnd, ID_TimerAnimation, 20, NULL);
        moveCell = *shape->GetCell(pointEmpty.x,pointEmpty.y);
        SetAnimationBackground();
    }
    countAnimation ++;
    MovingCell(&moveCell,shape->GetCell(pointCell.x,pointCell.y));
    form->DrawCell(&animationBackground,&moveCell,number);
    if(countAnimation == lengthAnimation)
    {
        KillTimer(hwnd,ID_TimerAnimation);
        countAnimation = 0;
        form->DrawCell(&animationBackground,shape->GetCell(pointCell.x,pointCell.y),number);
        if(puzzle->IsGameEnd())
        {
            state = GameStateEnd;
            GameEnd();
        }
        else
        {
            state = GameStateActive;
        }
    }
}

void Game::Resize(int width, int height)
{
    shape->Calculate(width, height, infoPanelHeight);
    _width = width;
    _height = height;
}

void Game::Restart()
{
    if(state == GameStateEnd)
    {
        SetTimer(hwnd,ID_TimerClock,1000,NULL);
    }
    score = 0;
    Score();
    InfoPanel();
    counterStart = GetTickCount();
    state = GameStateActive;
    puzzle->StartNewGame();
    Draw();
}

void Game::SetBackgroundImage(Gdiplus::Bitmap *bmp)
{
    delete form;
    form = new FormImage(hwnd, infoPanelHeight, bmp);
    Draw();
}

void Game::SetBackgroundNumber()
{
    delete form;
    form = new FormNumber(hwnd, infoPanelHeight);
    Draw();
}

void Game::ShowRightCombination()
{
    if(state == GameStateActive)
    {
        form->DrawRightCombination(shape,_width,_height);
        state = GameStatePause;
    }
    else if(state == GameStatePause)
    {
        state = GameStateActive;
        Draw();
    }
}

int Game::GetInfoPanelHeight()
{
    return infoPanelHeight;
}

void Game::InfoPanel()
{
    int counter = GetTickCount() - counterStart;
    counter /= 1000;
    if(counter >= 60)
    {
        WCHAR str[5];
        strTime[0] = 0;
        if(counter >= 3600)
        {
            int h = counter / 3600;
            counter %= 3600;
            _itow(h,str,10);
            wcscat(strTime,str);
        }
        int min = counter / 60;
        if(min < 10)
        {
            strTime[0] = '0';
            strTime[1] = 0;
        }
        counter %= 60;
        _itow(min,str,10);
        wcscat(strTime,str);
        if(counter < 10)
        {
            wcscat(strTime,L":0");
        }
        else
        {
            wcscat(strTime,L":");
        }
        _itow(counter,str,10);
        wcscat(strTime,str);
    }
    else
    {
        _itow(counter,strTime,10);
    }
    form->DrawInfoPanel(_width,strTime,strScore);
}

void Game::SetAnimationBackground()
{
    POINT pointEmpty = puzzle->GetEmpty();
    POINT pointCell = puzzle->GetLastMoved();
    LPRECT empty = shape->GetCell(pointEmpty.x,pointEmpty.y);
    LPRECT cell = shape->GetCell(pointCell.x,pointCell.y);
    if(empty->left < cell->left)
    {
        animationBackground.left = empty->left;
        animationBackground.right = cell->left - empty->left + cell->right;
        animationBackground.top = empty->top;
        animationBackground.bottom = empty->bottom;
    }
    else if(empty->left == cell->left)
    {
        animationBackground.left = empty->left;
        animationBackground.right = empty->right;
        if(empty->top < cell->top)
        {
            animationBackground.top = empty->top;
            animationBackground.bottom = cell->top - empty->top + cell->bottom;
        }
        else
        {
            animationBackground.top = cell->top;
            animationBackground.bottom = empty->top - cell->top + empty->bottom;
        }
    }
    else
    {
        animationBackground.left = cell->left;
        animationBackground.right = empty->left - cell->left + empty->right;
        animationBackground.top = cell->top;
        animationBackground.bottom = cell->bottom;
    }
}

void Game::MovingCell(RECT *animatedCell, RECT *emptyCell)
{
    if(animatedCell->left == emptyCell->left)
    {
        if(animatedCell->top > emptyCell->top)
        {
            animatedCell->top -= animatedCell->bottom / lengthAnimation;
        }
        else
        {
            animatedCell->top += animatedCell->bottom / lengthAnimation;
        }
    }
    else
    {
        if(animatedCell->left > emptyCell->left)
        {
            animatedCell->left -= animatedCell->right / lengthAnimation;
        }
        else
        {
            animatedCell->left += animatedCell->right / lengthAnimation;
        }
    }
}

void Game::Score()
{
    _itow(score,strScore,10);
}

void Game::GameEnd()
{
    KillTimer(hwnd,ID_TimerClock);
    Draw();
    MessageBox(hwnd,"Поздравляем с победой!","Победа",MB_OK);
}

bool Game::IsInRegion(int regionX, int regionY, int pointX, int pointY)
{
    if(regionX >= 0 && regionX < 4 && regionY < 4 && regionY >= 0)
    {
        LPRECT rect = shape->GetCell(regionX, regionY);
        if(rect->left < pointX && rect->left + rect->right > pointX)
        {
            if(rect->top < pointY && rect->top + rect->bottom > pointY)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

Game::~Game(void)
{
    delete puzzle;
    delete shape;
    delete form;
    KillTimer(hwnd,ID_TimerClock);
}
