#include "Puzzle.h"
#include "Shape.h"
#include "FormNumber.h"
#include "FormImage.h"
#include <gdiplus.h>

#pragma once
class Game
{
private:
    Puzzle *puzzle;
    Shape *shape;
    Form *form;
    int _width, _height;
    HWND hwnd;
    int countAnimation;
    int lengthAnimation;
    int infoPanelHeight;
    int score;
    int counterStart;
    RECT moveCell;
    RECT animationBackground;
    enum State {GameStateActive,GameStatePause,GameStateAnimation,GameStateEnd};
    State state;
    WCHAR strTime[10], strScore[5];

    void SetAnimationBackground();
    void MovingCell(RECT *animatedCell, RECT *emptyCell);
    void Score();
    void GameEnd();
    bool IsInRegion(int regionX, int regionY, int pointX, int pointY);
public:
    Game(HWND hWnd, int width, int height);
    void Left();
    void Right();
    void Up();
    void Down();
    void MouseClick(int x, int y);
    void Draw();
    void Animation();
    void Resize(int width, int height);
    void Restart();
    void SetBackgroundImage(Gdiplus::Bitmap *bmp);
    void SetBackgroundNumber();
    void ShowRightCombination();
    int GetInfoPanelHeight();
    void InfoPanel();
    const static int ID_TimerAnimation = 1001;
    const static int ID_TimerClock = 1002;
    ~Game(void);
};

