#include "TXLib.h"

void MoveBall  ();

void PhysicsBall (int* x, int* y, int* vx, int* vy, int* ax, int* ay, int dt, int r);
void DrawBall    (int  x, int  y, int  vx, int  vy, int   r, COLORREF color, COLORREF fillcolor);
void Collision   (int x1, int y1, int  x2, int  y2, int* vx1, int* vy1, int* vx2, int* vy2, int r1, int r2, int* hit);
void ControlBall (int* vx, int* vy);
void ScoreDraw   (int hits);

int main()
    {
    txCreateWindow (800, 600);

    MoveBall ();

    return 0;
    }

void MoveBall ()
    {
    int x1  = 50, y1  = 50,
        vx1 =  3, vy1 =  2,
        ax1 =  0, ay1 =  1,
        r1  = 20;

    int x2  = 100, y2  = 100,
        vx2 =   4, vy2 =   3,
        ax2 =   0, ay2 =   1,
        r2  =  30;

    int dt  = 1;
    int hit = 0;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        txSetFillColor (TX_BLACK);
        txClear();

        ScoreDraw (hit);

        DrawBall (x1, y1, vx1, vy1, r1, TX_LIGHTRED, TX_RED);
        DrawBall (x2, y2, vx2, vy2, r2, TX_LIGHTBLUE, TX_BLUE);

        PhysicsBall (&x1, &y1, &vx1, &vy1, &ax1, &ay1, dt, r1);
        PhysicsBall (&x2, &y2, &vx2, &vy2, &ax2, &ay2, dt, r2);

        ControlBall (&x1, &y1);

        Collision   (x1, y1, x2, y2, &vx1, &vy1, &vx2, &vy2, r1, r2, &hit);

        txSleep (10);
        }
     }

//-------------------------------------------------------------

void PhysicsBall (int* x, int* y, int* vx, int* vy, int* ax, int* ay, int dt, int r)
    {
    *vx += (*ax) * dt;
    *vy += (*ay) * dt;

    *x  += (*vx)  * dt;
    *y  += (*vy)  * dt;

    if (*x > 800 - r)
        {
        *vx = -(*vx);
        *x  = 800 - r;
        }

    if (*y > 600 - r)
        {
        *vy = -(*vy);
        *y  = 600 - r;
        }

    if (*x < 0 + r)
        {
        *vx = -(*vx);
        *x  = 0 + r;
        }

    if (*y < 0 + r)
        {
        *vy = -(*vy);
        *y  = 0 + r;
        }
    }

//-------------------------------------------------------------

void ControlBall (int* vx, int* vy)
    {
    if (txGetAsyncKeyState (VK_RIGHT)) *vx = *vx + 1;
    if (txGetAsyncKeyState (VK_LEFT))  *vx = *vx - 1;
    if (txGetAsyncKeyState (VK_UP))    *vy = *vy - 1;
    if (txGetAsyncKeyState (VK_DOWN))  *vy = *vy + 1;

    if (txGetAsyncKeyState (VK_SPACE)) *vx = *vy = 0;
    }

//-------------------------------------------------------------

void DrawBall (int x, int y, int vx, int vy, int r, COLORREF color, COLORREF fillcolor)
    {
    txSetColor (color, 2);
    txSetFillColor (fillcolor);

    txCircle (x, y, r);
    txLine   (x, y, x + vx*5, y + vy*5);
    txCircle (x + vx*5, y + vy*5, 3);
    }

//-------------------------------------------------------------

void ScoreDraw (int hits)
    {
    txSelectFont ("TimesNewRoman", 50);
    char str[12] = "";
    sprintf (str, "Hits = %d", hits);

    int centerX = txGetExtentX()/2;

    int textSizeX = txGetTextExtentX ("Hits = ---"),
        textSizeY = txGetTextExtentY ("Hits = ---");

    txSetFillColor (TX_BLACK);
    txRectangle (centerX - textSizeX, 0,
                 centerX + textSizeX, textSizeY);
    txTextOut   (centerX - textSizeX/2, 0, str);
    }

//-------------------------------------------------------------

void Collision (int x1, int y1, int x2, int y2, int* vx1, int* vy1, int* vx2, int* vy2, int r1, int r2, int* hit)
    {
    if (pow(x1 - x2, 2) + pow(y1 - y2, 2) <= r1*r1 + 2*r1*r2 + r2*r2)
        {
        *vx2 = -(*vx2);
        *vy2 = -(*vy2);
        *vx1 = -(*vx1);
        *vy1 = -(*vy1);

        txPlaySound ("sounds/ball_sound.wav");

        *hit = *hit + 1;
        }
    }
