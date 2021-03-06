#include "TXLib.h"

void MoveBall  (HDC fon);

void PhysicsBall (int* x, int* y, int* vx, int* vy, int* ax, int* ay, int dt, int r);
void DrawBall    (int  x, int  y, int  vx, int  vy, int   r, COLORREF color, COLORREF fillcolor);
void Collision   (int x1, int y1, int  x2, int  y2,
                  int* vx1, int* vy1, int* vx2, int* vy2, int r1, int r2);
void ControlBall (int* vx1, int* vy1, int* vx3, int* vy3);
void ScoreDraw   (int score1, int score2);
void ScoreCount  (int x, int y, int r2, int* score1, int* score2);

int main()
    {
    txCreateWindow (800, 600);

    HDC fon  = txLoadImage ("images\\duet+gate.bmp");

    MoveBall (fon);

    return 0;
    }

void MoveBall (HDC fon)
    {
    int x1  = 50, y1  = 50,
        vx1 =  3, vy1 =  2,
        ax1 =  0, ay1 =  1,
        r1  = 20;

    int x2  = 100, y2  = 100,
        vx2 =   4, vy2 =   3,
        ax2 =   0, ay2 =   1,
        r2  =  30;

    int x3  = 550, y3  = 50,
        vx3 =  -3, vy3 =  2,
        ax3 =   0, ay3 =  1,
        r3  =  20;

    int dt     = 1;
    int score1 = 0;
    int score2 = 0;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        //txSetFillColor (TX_BLACK);
        //txClear();

        txBitBlt  (txDC(), 0, 0, 800, 600, fon, 0, 0);

        ScoreDraw (score1, score2);

        DrawBall  (x1, y1, vx1, vy1, r1, TX_LIGHTRED, TX_RED);
        DrawBall  (x2, y2, vx2, vy2, r2, TX_LIGHTBLUE, TX_BLUE);
        DrawBall  (x3, y3, vx3, vy3, r3, TX_GREEN, TX_GREEN);

        PhysicsBall (&x1, &y1, &vx1, &vy1, &ax1, &ay1, dt, r1);
        PhysicsBall (&x2, &y2, &vx2, &vy2, &ax2, &ay2, dt, r2);
        PhysicsBall (&x3, &y3, &vx3, &vy3, &ax3, &ay3, dt, r3);

        ControlBall (&x1, &y1, &x3, &y3);

        Collision   (x1, y1, x2, y2, &vx1, &vy1, &vx2, &vy2, r1, r2);
        Collision   (x3, y3, x2, y2, &vx3, &vy3, &vx2, &vy2, r1, r2);
        Collision   (x1, y1, x3, y3, &vx1, &vy1, &vx3, &vy3, r1, r2);

        ScoreCount (x2, y2, r2, &score1, &score2);

        txSleep (10);
        }

    txDeleteDC (fon);
    }

//-------------------------------------------------------------

void PhysicsBall (int* x, int* y, int* vx, int* vy, int* ax, int* ay, int dt, int r)
    {
    *vx += (*ax) * dt;
    *vy += (*ay) * dt;

    *x  += (*vx) * dt;
    *y  += (*vy) * dt;

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

void ControlBall (int* vx1, int* vy1, int* vx3, int* vy3)
    {
    if (txGetAsyncKeyState (VK_RIGHT)) *vx1 = *vx1 + 1;
    if (txGetAsyncKeyState (VK_LEFT))  *vx1 = *vx1 - 1;
    if (txGetAsyncKeyState (VK_UP))    *vy1 = *vy1 - 1;
    if (txGetAsyncKeyState (VK_DOWN))  *vy1 = *vy1 + 1;

    if (txGetAsyncKeyState ('F')) *vx3 = *vx3 + 1;
    if (txGetAsyncKeyState ('A')) *vx3 = *vx3 - 1;
    if (txGetAsyncKeyState ('W')) *vy3 = *vy3 - 1;
    if (txGetAsyncKeyState ('S')) *vy3 = *vy3 + 1;

    if (txGetAsyncKeyState (VK_SPACE)) *vx1 = *vy1 = *vx3 = *vy3 = 0;
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

void ScoreDraw (int score1, int score2)
    {
    txSelectFont ("TimesNewRoman", 50);
    char str[12] = "";
    sprintf (str, "%d : %d", score1, score2);

    int centerX = txGetExtentX()/2;

    int textSizeX = txGetTextExtentX ("------"),
        textSizeY = txGetTextExtentY ("------");

    txSetFillColor (TX_BLACK);
    txRectangle (centerX - textSizeX, 0,
                 centerX + textSizeX, textSizeY);
    txTextOut   (centerX - textSizeX/2, 0, str);
    }

//-------------------------------------------------------------

void Collision (int x1, int y1, int x2, int y2, int* vx1, int* vy1, int* vx2, int* vy2, int r1, int r2)
    {
    if (pow(x1 - x2, 2) + pow(y1 - y2, 2) <= r1*r1 + 2*r1*r2 + r2*r2)
        {
        *vx2 = -(*vx2);
        *vy2 = -(*vy2);
        *vx1 = -(*vx1);
        *vy1 = -(*vy1);

        txPlaySound ("sounds/ball_sound.wav");
        }
    }

//-------------------------------------------------------------

void ScoreCount (int x, int y, int r2, int* score1, int* score2)
    {
    if ((355 > y) && (y > 240) && (x + r2 > 799))
        {
        *score1 = *score1 + 1;
        }
    if ((y > 240) && (y < 355) && (x - r2 < 1))
        {
        *score2 = *score2 + 1;
        }
    }
