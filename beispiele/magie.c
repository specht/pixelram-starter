// Palette Cycling: Der Zauberkreis wird ein einziges Mal berechnet. In
// der Schleife ändern sich nur die acht Farben 8 bis 15 der Palette, und
// trotzdem scheint Energie durch das Muster zu laufen.
//
// Die Kanten sind geglättet: Jedes Pixel merkt sich die Paletten-Nummern
// an vier Punkten in seinem Inneren. Gemischte Farben können nicht durch
// die Palette wandern, deshalb mischt das Programm in jedem Bild die
// aktuellen Farben dieser vier Nummern neu. Die Form rechnet es nie wieder.
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

// die Paletten-Nummer an einem Punkt des Zauberkreises
static int magic_color(double x, double y)
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

    return color;
}

// vier Paletten-Nummern für jedes Pixel, einmal berechnet
static uint8_t samples[H][W][4];

static void compute_circle(void)
{
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            for (int i = 0; i < 4; i++)
                samples[y][x][i] = magic_color(x + 0.25 + (i % 2) * 0.5, y + 0.25 + (i / 2) * 0.5);
}

// mischt für jedes Pixel die aktuellen Farben seiner vier Nummern
static void draw_circle(void)
{
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
        {
            int sum_r = 0, sum_g = 0, sum_b = 0;
            for (int i = 0; i < 4; i++)
            {
                uint8_t r, g, b;
                get_palette(samples[y][x][i], &r, &g, &b);
                sum_r += r;
                sum_g += g;
                sum_b += b;
            }
            set_pixel_rgb(x, y, sum_r / 4, sum_g / 4, sum_b / 4);
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
    screen_open(W, H, pixel_rgb24, "Magie");
    set_palette(0, 2, 3, 9);

    compute_circle();

    // Die Form steht fest: Nur die Palette bewegt sich, und das Bild
    // mischt ihre Farben neu.
    while (!should_close())
    {
        cycle_colors(seconds());
        draw_circle();
        present();
    }

    screen_close();
    return 0;
}
