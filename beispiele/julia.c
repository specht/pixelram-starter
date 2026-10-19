// Julia-Menge: Für jeden Pixel wird dieselbe kleine Rechnung immer wieder
// ausgeführt. Wie lange die Zahlen dabei klein bleiben, bestimmt die
// Farbe. cr und ci wandern langsam, deshalb verändert sich die Form.
// Die Kanten sind geglättet wie in »Glatte Kanten«: Wo die Ecken eines
// Pixels verschiedene Farben haben, wird es geteilt und gemischt.
//
// make PROGRAM=beispiele/julia

#include "pixelram.h"

#include <math.h>

#define W 320
#define H 180
#define ITER 48

// wie oft ein Pixel an einer Kante höchstens geteilt wird
#define LEVELS 1

// die Konstante der Julia-Menge, für jedes Bild neu
double cr, ci;

int color(double x, double y)
{
    double zx = (x - W / 2.0) * 3.0 / W;
    double zy = (y - H / 2.0) * 2.0 / H;
    int i = 0;

    while (zx * zx + zy * zy < 4 && i < ITER)
    {
        double xx = zx * zx - zy * zy + cr;
        zy = 2 * zx * zy + ci;
        zx = xx;
        i++;
    }

    return i == ITER ? 0 : 1 + i * 62 / ITER;
}

// die Farben an den Ecken der Pixel: eine Zeile und eine Spalte mehr
int corner[H + 1][W + 1];

// Die Farbe eines Quadrats mit der linken oberen Ecke (x, y) und der
// Seitenlänge size; c00, c10, c01 und c11 sind die Farben seiner Ecken.
void square(double x, double y, double size, int c00, int c10, int c01, int c11, int levels, int rgb[3])
{
    if (levels == 0 || (c00 == c10 && c00 == c01 && c00 == c11))
    {
        uint8_t r, g, b;
        get_palette(c00, &r, &g, &b);
        rgb[0] = r;
        rgb[1] = g;
        rgb[2] = b;
        return;
    }

    double half = size / 2;
    int top    = color(x + half, y);
    int left   = color(x,        y + half);
    int middle = color(x + half, y + half);
    int right  = color(x + size, y + half);
    int bottom = color(x + half, y + size);

    int upper_left[3], upper_right[3], lower_left[3], lower_right[3];
    square(x,        y,        half, c00,    top,    left,   middle, levels - 1, upper_left);
    square(x + half, y,        half, top,    c10,    middle, right,  levels - 1, upper_right);
    square(x,        y + half, half, left,   middle, c01,    bottom, levels - 1, lower_left);
    square(x + half, y + half, half, middle, right,  bottom, c11,    levels - 1, lower_right);

    for (int i = 0; i < 3; i++)
        rgb[i] = (upper_left[i] + upper_right[i] + lower_left[i] + lower_right[i]) / 4;
}

void draw(void)
{
    for (int y = 0; y <= H; y++)
        for (int x = 0; x <= W; x++)
            corner[y][x] = color(x, y);

    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
        {
            int rgb[3];
            square(x, y, 1.0, corner[y][x], corner[y][x + 1], corner[y + 1][x], corner[y + 1][x + 1], LEVELS, rgb);
            set_pixel_rgb(x, y, rgb[0], rgb[1], rgb[2]);
        }
}

int main(void)
{
    screen_open(W, H, pixel_rgb24, "Julia");
    use_palette("aap_64");

    while (!should_close())
    {
        double t = seconds();
        cr = -0.72 + cos(t * 0.17) * 0.08;
        ci = 0.27 + sin(t * 0.13) * 0.08;

        draw();
        present();
    }

    screen_close();
    return 0;
}
