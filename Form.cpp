#include "Form.h"

using namespace Gdiplus;

Form::Form(HWND hWnd, int infPanelH)
{
    hwnd = hWnd;
    infoPanelHeight = infPanelH;
    brushPanel = new SolidBrush(Color(100,100,100));
    brushText = new SolidBrush(Color(255,255,255));
}

void Form::DrawInfoPanel(int width, WCHAR *counter, WCHAR *score)
{
    HDC hdc;
    hdc = GetDC(hwnd);
    Graphics gr(hdc);
    Bitmap backBuffer(width,infoPanelHeight,&gr);
    Graphics temp(&backBuffer);
    temp.FillRectangle(brushPanel,0,0,width,infoPanelHeight);
    Font font(L"Arial",18);
    temp.DrawString(counter,-1,&font,PointF(0,0),brushText);
    temp.DrawString(score,-1,&font,PointF(width - 70,0),brushText);
    gr.DrawImage(&backBuffer,0,0);
    ReleaseDC(hwnd, hdc);
}

Form::~Form(void)
{
    delete brushPanel;
    delete brushText;
}
