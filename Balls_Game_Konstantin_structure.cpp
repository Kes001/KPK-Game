#include "TXLib.h"

void MoveBall  (HDC fon);

struct Ball
    {
    int x,  y,
        vx, vy;
    int r;

    COLORREF color;
    COLORREF fillcolor;
    };

struct ControlType
    {
    int key_left, key_right, key_up, key_down;
    };

void PhysicsBall (struct Ball* ball, int ax, int ay, int dt);
void DrawBall    (struct Ball ball);
void Collision   (struct Ball* ball1, struct Ball* ball2);
void ControlBall (struct Ball* ball,  struct ControlType);
void ScoreDraw   (int score1, int score2);

bool LeftGool   (struct Ball ball);
bool RightGool  (struct Ball ball);
double Distance (struct Ball ball1, struct Ball ball2);

int main()
    {
    txCreateWindow (800, 600);

    HDC fon  = txLoadImage ("images\\duet+gate.bmp");

    MoveBall (fon);

    return 0;
    }

//-------------------------------------------------------------

void MoveBall (HDC fon)
    {
    Ball ball1 = {  50,  50,  3, 2, 20, TX_LIGHTRED, TX_RED };
    Ball ball2 = { 100, 100,  4, 3, 30, TX_LIGHTBLUE, TX_BLUE };
    Ball ball3 = { 550,  50, -3, 2, 20, TX_GREEN, TX_GREEN };

    ControlType player1 = {'D', 'A', 'W', 'S'};
    ControlType player2 = {VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN};

    int dt     = 1;
    int score1 = 0, score2 = 0;
    int     ax = 0,     ay = 1;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        txBegin ();
        txBitBlt  (txDC(), 0, 0, 800, 600, fon, 0, 0);

        ScoreDraw (score1, score2);

        DrawBall  (ball1);
        DrawBall  (ball2);
        DrawBall  (ball3);

        PhysicsBall (&ball1, ax, ay, dt);
        PhysicsBall (&ball2, ax, ay, dt);
        PhysicsBall (&ball3, ax, ay, dt);

        ControlBall (&ball1, player1);
        ControlBall (&ball3, player2);

        if (Distance (ball1, ball2) <= ball1.r + ball2.r) Collision (&ball1, &ball2);
        if (Distance (ball2, ball3) <= ball2.r + ball3.r) Collision (&ball2, &ball3);
        if (Distance (ball1, ball3) <= ball1.r + ball3.r) Collision (&ball1, &ball3);

        if (LeftGool  (ball2)) score1++;
        if (RightGool (ball2)) score2++;

        txEnd ();
        txSleep (10);
        }

    txDeleteDC (fon);
    }

//-------------------------------------------------------------

double Distance (struct Ball ball1, struct Ball ball2)
    {
    return sqrt ((ball1.x - ball2.x)*(ball1.x - ball2.x) + (ball1.y - ball2.y)*(ball1.y - ball2.y));
    }

//-------------------------------------------------------------

void PhysicsBall (struct Ball* ball, int ax, int ay, int dt)
    {
    (*ball).vx = (*ball).vx + ax * dt;
    (*ball).vy = (*ball).vy + ay * dt;

    (*ball).x  = (*ball).x + (*ball).vx * dt;
    (*ball).y  = (*ball).y + (*ball).vy * dt;

    if ((*ball).x > 800 - (*ball).r)
        {
        (*ball).vx = -(*ball).vx;
        (*ball).x  = 800 - (*ball).r;
        }

    if ((*ball).y > 600 - (*ball).r)
        {
        (*ball).vy = -(*ball).vy;
        (*ball).y  = 600 - (*ball).r;
        }

    if ((*ball).x < 0 + (*ball).r)
        {
        (*ball).vx = -(*ball).vx;
        (*ball).x  = 0 + (*ball).r;
        }

    if ((*ball).y < 0 + (*ball).r)
        {
        (*ball).vy = -(*ball).vy;
        (*ball).y  = 0 + (*ball).r;
        }
    }

//-------------------------------------------------------------

void ControlBall (struct Ball* ball, struct ControlType player)
    {
    if (txGetAsyncKeyState (player.key_right)) (*ball).vx = (*ball).vx - 1;
    if (txGetAsyncKeyState (player.key_left))  (*ball).vx = (*ball).vx + 1;
    if (txGetAsyncKeyState (player.key_up))    (*ball).vy = (*ball).vy - 1;
    if (txGetAsyncKeyState (player.key_down))  (*ball).vy = (*ball).vy + 1;

    if (txGetAsyncKeyState (VK_SPACE))  (*ball).vx = (*ball).vy =  0;
    }

//-------------------------------------------------------------

void DrawBall (struct Ball ball)
    {
    txBegin ();
    txSetColor (ball.color, 2);
    txSetFillColor (ball.fillcolor);

    txCircle (ball.x, ball.y, ball.r);
    txLine   (ball.x, ball.y, ball.x + ball.vx*5, ball.y + ball.vy*5);
    txCircle (ball.x + ball.vx*5, ball.y + ball.vy*5, 3);
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

void Collision   (struct Ball* ball1, struct Ball* ball2)
    {
    (*ball1).vx = -(*ball1).vx;
    (*ball1).vy = -(*ball1).vy;
    (*ball2).vx = -(*ball2).vx;
    (*ball2).vy = -(*ball2).vy;

    txPlaySound ("sounds/ball_sound.wav");
    }

//-------------------------------------------------------------

bool LeftGool (struct Ball ball)
    {
    return (355 > ball.y) && (ball.y > 240) && (ball.x + ball.r >= 800);
    }

//-------------------------------------------------------------

bool RightGool (struct Ball ball)
    {
    return (ball.y > 240) && (ball.y < 355) && (ball.x - ball.r <= 0);
    }
