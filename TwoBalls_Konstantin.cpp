#include "TXLib.h"

void MoveBalls();

int main()
    {
    txCreateWindow (400, 300);

    MoveBalls();

    return 0;
    }

void MoveBalls()
    {
    int x  = 50, y  = 50, x1  = 750, y1  = 550,
        vx =  3, vy =  3, vx1 =   3, vy1 =   3,
        r  = 20;

    int dt  = 1;
    int hit = 0;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        txSetFillColor (TX_BLACK);
        txClear();

        txCircle (x,  y,  r);
        txCircle (x1, y1, r);

        x  += vx  * dt;
        y  += vy  * dt;
        x1 += vx1 * dt;
        y1 += vy1 * dt;

        if (pow(x1 - x, 2) + pow(y1 - y, 2) <= 4*r*r)
            {
            vx  = -vx;
            vy  = -vy;
            vx1 = -vx1;
            vy1 = -vy1;

            txPlaySound    ("sounds/ball_sound.wav");
            txSetColor     (RGB (x,   y,   100), 2);
            txSetFillColor (RGB (x/2, y/2, 100));
            hit++;
            }

        if (x > 400 - r)
            {
            vx = -vx;
            x  = 400 - r;
            }

        if (y > 300 - r)
            {
            vy = -vy;
            y  = 300 - r;
            }

        if (x < 0 + r)
            {
            vx = -vx;
            x  = 0 + r;
            }

        if (y < 0 + r)
            {
            vy = -vy;
            y  = 0 + r;
            }

        if (txGetAsyncKeyState (VK_RIGHT)) x ++;
        if (txGetAsyncKeyState (VK_LEFT))  x --;
        if (txGetAsyncKeyState (VK_UP))    y --;
        if (txGetAsyncKeyState (VK_DOWN))  y ++;

        if (x1 > 400 - r)
            {
            vx1 = -vx1;
            x1  = 400 - r;
            }

        if (y1 > 300-r)
            {
            vy1 = -vy1;
            y1  = 300 - r;
            }

        if (x1 < 0 + r)
            {
            vx1 = -vx1;
            x1  = 0 + r;
            }

        if (y1 < 0 + r)
            {
            vy1 = -vy1;
            y1  = 0 + r;
            }

        txSleep (5);
        }
    printf("Hits = %d\n", hit);
    }

