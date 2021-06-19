#include "TXLib.h"

void MoveBall  (HDC fon);

struct Ball
    {
    int x,  y,
        vx, vy;
    int r;

    COLORREF color;
    COLORREF fillcolor;

    void Draw    ();
    void Physics (double ax, double ay, int dt);

    bool LeftGool  ();
    bool RightGool ();
    };

struct ControlType
    {
    int key_left, key_right, key_up, key_down;
    };

void Collision   (struct Ball* ball1, struct Ball* ball2, int dt);
void ControlBall (struct Ball* ball,  struct ControlType);
void ScoreDraw   (int score1, int score2);

double Distance  (struct Ball ball1, struct Ball ball2);

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

    int dt = 1;
    double ax = 0.0, ay = 0.5;
    int score1 = 0, score2 = 0;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        txBegin ();
        txBitBlt  (txDC(), 0, 0, 800, 600, fon, 0, 0);

        ScoreDraw (score1, score2);

        ball1.Draw ();
        ball2.Draw ();
        ball3.Draw ();

        ball1.Physics (ax, ay, ROUND(dt));
        ball2.Physics (ax, ay, ROUND(dt));
        ball3.Physics (ax, ay, ROUND(dt));

        ControlBall (&ball1, player1);
        ControlBall (&ball3, player2);

        if (Distance (ball1, ball2) <= ball1.r + ball2.r) Collision (&ball1, &ball2, dt);
        if (Distance (ball2, ball3) <= ball2.r + ball3.r) Collision (&ball2, &ball3, dt);
        if (Distance (ball1, ball3) <= ball1.r + ball3.r) Collision (&ball1, &ball3, dt);

        if (ball2.LeftGool  ())  score1++;
        if (ball2.RightGool ()) score2++;

        txEnd ();
        txSleep (10);
        }

    txDeleteDC (fon);
    }

//-------------------------------------------------------------

double Distance (struct Ball ball1, struct Ball ball2)
    {
    double d = 0;
    d = sqrt ((ball1.x - ball2.x)*(ball1.x - ball2.x) + (ball1.y - ball2.y)*(ball1.y - ball2.y));
    if (d < 0.00001) return 0.01;
    return d;
    }

//-------------------------------------------------------------

void Ball::Physics (double ax, double ay, int dt)
    {
    (*this).vx = (*this).vx + ROUND(ax * dt);
    (*this).vy = (*this).vy + ROUND(ay * dt);

    x = x + ROUND(vx * dt);
    y = y + ROUND(vy * dt);

    if (x > 800 - r)
        {
        vx = -vx;
        x  = 800 - r;
        }

    if ((*this).y > 600 - (*this).r)
        {
        (*this).vy = -(*this).vy;
        (*this).y  = 600 - (*this).r;
        }

    if ((*this).x < 0 + (*this).r)
        {
        (*this).vx = -(*this).vx;
        (*this).x  = 0 + (*this).r;
        }

    if ((*this).y < 0 + (*this).r)
        {
        (*this).vy = -(*this).vy;
        (*this).y  = 0 + (*this).r;
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

void Ball::Draw ()
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

void Collision   (struct Ball* ball1, struct Ball* ball2, int dt)
    {
    /*ball1 -> vx = -ball1 -> vx;
    ball1 -> vy = -ball1 -> vy;
    ball2 -> vx = -ball2 -> vx;
    ball2 -> vy = -ball2 -> vy;
    */

    float Dx = ball1 -> x - ball2 -> x;
    float Dy = ball1 -> y - ball2 -> y;
    float d = Distance (*ball1, *ball2);
    float s = Dx/d; // sin
    float e = Dy/d; // cos

    float Vn1 = (ball2 -> vx)*dt*s + (ball2 -> vy)*dt*e;
    float Vn2 = (ball1 -> vx)*dt*s + (ball1 -> vy)*dt*e;


    float Vt1 = -(ball2 -> vx)*dt*e + (ball2 -> vy)*dt*s;
    float Vt2 = -(ball1 -> vx)*dt*e + (ball1 -> vy)*dt*s;

    float o = Vn2;
    Vn2 = Vn1;
    Vn1 = o;

    ball1 -> vx = Vn2*s - Vt2*e;
    ball1 -> vy = Vn2*e + Vt2*s;
    ball2 -> vx = Vn1*s - Vt1*e;
    ball2 -> vy = Vn1*e + Vt1*s;


    txPlaySound ("sounds/ball_sound.wav");
    }

//-------------------------------------------------------------

bool Ball::LeftGool ()
    {
    return (355 > y) && (y > 240) && (x + r >= 800);
    }

//-------------------------------------------------------------

bool Ball::RightGool ()
    {
    return (y > 240) && (y < 355) && (x - r <= 0);
    }
