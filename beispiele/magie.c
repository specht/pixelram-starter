// Palette Cycling: Der Zauberkreis wird ein einziges Mal gezeichnet. In
// der Schleife ändern sich nur die acht Farben 8 bis 15 der Palette, und
// trotzdem scheint Energie durch das Muster zu laufen.
//
// make PROGRAM=beispiele/magie

#include "pixelram.h"

#include <math.h>

#define W 320
#define H 180

static const uint8_t magic[8][3] = {
    { 12,  10,  45},
    { 35,  20, 100},
    { 85,  30, 180},
    {190,  45, 220},
    {255, 180, 255},
    {150, 245, 255},
    { 30, 180, 255},
    { 15,  70, 160},
};

static void draw_circle(void)
{
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
        {
            double dx = x - W / 2.0, dy = y - H / 2.0;
            double r = hypot(dx, dy);
            double a = atan2(dy, dx);
            int color = 0;

            // äußerer Ring
            if (r > 67 && r < 71)
                color = 8 + ((int)(a * 8 + 40) & 7);
            // zwölf Strahlen
            if (r > 51 && r < 64 && fabs(sin(a * 12)) < .16)
                color = 8 + ((int)(r / 3 + a * 5) & 7);
            // zwei Zauberringe
            if ((r > 43 && r < 47) || (r > 29 && r < 33))
                color = 8 + ((int)(a * 9 + r / 3) & 7);
            // geschwungene Energie im Inneren
            if (r < 28 && fabs(sin(r * .34 + a * 5)) < .38)
                color = 8 + ((int)(r / 2 + a * 4) & 7);
            // heller Kern
            if (r < 9)
                color = 8 + ((int)(r + a * 3) & 7);

            set_pixel(x, y, color);
        }
}

static void cycle_colors(double t)
{
    double p = fmod(t * 5.0, 8.0);
    int shift = (int)p;
    double f = p - shift;

    for (int i = 0; i < 8; i++)
    {
        int a = (i + shift) & 7;
        int b = (a + 1) & 7;
        set_palette(8 + i,
            magic[a][0] + (magic[b][0] - magic[a][0]) * f,
            magic[a][1] + (magic[b][1] - magic[a][1]) * f,
            magic[a][2] + (magic[b][2] - magic[a][2]) * f);
    }
}

int main(void)
{
    screen_open(W, H, pixel_indexed8, "Magie");
    set_palette(0, 2, 3, 9);

    cycle_colors(0);
    draw_circle();

    // Kein einziges set_pixel mehr: nur die Palette bewegt sich.
    while (!should_close())
    {
        cycle_colors(seconds());
        present();
    }

    screen_close();
    return 0;
}
