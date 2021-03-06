#include "TXLib.h"

void MoveBall  (HDC fon);

void PhysicsBall (int* x, int* y, int* vx, int* vy, int* ax, int* ay, int dt, int r);
void DrawBall    (int  x, int  y, int  vx, int  vy, int   r, COLORREF color, COLORREF fillcolor);
void Collision   (int* vx1, int* vy1, int* vx2, int* vy2);
void ControlBall (int* vx1, int* vy1, int* vx3, int* vy3);
void ScoreDraw   (int score1, int score2);

bool LeftGool  (int x, int y, int r2);
bool RightGool (int x, int y, int r2);
double Distance (double x1, double y1, double x2, double y2);

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
        txBegin ();
        txBitBlt  (txDC(), 0, 0, 800, 600, fon, 0, 0);

        ScoreDraw (score1, score2);

        DrawBall  (x1, y1, vx1, vy1, r1, TX_LIGHTRED, TX_RED);
        DrawBall  (x2, y2, vx2, vy2, r2, TX_LIGHTBLUE, TX_BLUE);
        DrawBall  (x3, y3, vx3, vy3, r3, TX_GREEN, TX_GREEN);

        PhysicsBall (&x1, &y1, &vx1, &vy1, &ax1, &ay1, dt, r1);
        PhysicsBall (&x2, &y2, &vx2, &vy2, &ax2, &ay2, dt, r2);
        PhysicsBall (&x3, &y3, &vx3, &vy3, &ax3, &ay3, dt, r3);

        ControlBall (&x1, &y1, &x3, &y3);

        if (Distance (x1, y1, x2, y2) <= r1 + r2) Collision (&vx1, &vy1, &vx2, &vy2);
        if (Distance (x3, y3, x2, y2) <= r2 + r3) Collision (&vx3, &vy3, &vx2, &vy2);
        if (Distance (x1, y1, x3, y3) <= r1 + r3) Collision (&vx1, &vy1, &vx3, &vy3);

        if (LeftGool  (x2, y2, r2)) score1++;
        if (RightGool (x2, y2, r2)) score2++;

        txEnd ();
        txSleep (10);
        }

    txDeleteDC (fon);
    }

//-------------------------------------------------------------

double Distance (double x1, double y1, double x2, double y2)
    {
    return sqrt ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
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
    if (txGetAsyncKeyState (VK_RIGHT)) *vx1 = *vx1 + 2;
    if (txGetAsyncKeyState (VK_LEFT))  *vx1 = *vx1 - 2;
    if (txGetAsyncKeyState (VK_UP))    *vy1 = *vy1 - 2;
    if (txGetAsyncKeyState (VK_DOWN))  *vy1 = *vy1 + 2;

    if (txGetAsyncKeyState ('D')) *vx3 = *vx3 + 2;
    if (txGetAsyncKeyState ('A')) *vx3 = *vx3 - 2;
    if (txGetAsyncKeyState ('W')) *vy3 = *vy3 - 2;
    if (txGetAsyncKeyState ('S')) *vy3 = *vy3 + 2;

    if (txGetAsyncKeyState (VK_SPACE)) *vx1 = *vy1 =   0;
    if (txGetAsyncKeyState (VK_SPACE)) *vx3 = *vy3 = 400;
    }

//-------------------------------------------------------------

void DrawBall (int x, int y, int vx, int vy, int r, COLORREF color, COLORREF fillcolor)
    {
    txBegin ();
    txSetColor (color, 2);
    txSetFillColor (fillcolor);

    txCircle (x, y, r);
    txLine   (x, y, x + vx*5, y + vy*5);
    txCircle (x + vx*5, y + vy*5, 3);
    txEnd ();
    }

//-------------------------------------------------------------

void ScoreDraw (int score1, int score2)
    {
    txBegin ();
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
    txEnd ();
    }

//-------------------------------------------------------------

void Collision (int* vx1, int* vy1, int* vx2, int* vy2)
    {
    *vx2 = -(*vx2);
    *vy2 = -(*vy2);
    *vx1 = -(*vx1);
    *vy1 = -(*vy1);

    txPlaySound ("sounds/ball_sound.wav");
    }

//-------------------------------------------------------------

bool LeftGool (int x, int y, int r2)
    {
    return (355 > y) && (y > 240) && (x + r2 >= 800);
    }

//-------------------------------------------------------------

bool RightGool (int x, int y, int r2)
    {
    return (y > 240) && (y < 355) && (x - r2 <= 0);
    }
