// Julia-Menge: Für jeden Pixel wird dieselbe kleine Rechnung immer wieder
// ausgeführt. Wie lange die Zahlen dabei klein bleiben, bestimmt die
// Farbe. cr und ci wandern langsam, deshalb verändert sich die Form.
//
// make PROGRAM=beispiele/julia

#include "pixelram.h"

#include <math.h>

#define W 320
#define H 180
#define ITER 48

int main(void)
{
    screen_open(W, H, pixel_indexed8, "Julia");
    use_palette("aap_64");

    while (!should_close())
    {
        double t = seconds();
        double cr = -0.72 + cos(t * 0.17) * 0.08;
        double ci = 0.27 + sin(t * 0.13) * 0.08;

        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++)
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

                set_pixel(x, y, i == ITER ? 0 : 1 + i * 62 / ITER);
            }

        present();
    }

    screen_close();
    return 0;
}
