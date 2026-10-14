// Tunnel: Aus Abstand und Winkel jedes Pixels zur Mitte entstehen Ringe
// und Streifen, die auf dich zukommen. Zwischen den Farben der Palette
// wird weich übergeblendet, deshalb läuft der Bildschirm im RGB-Modus.
//
// make PROGRAM=beispiele/tunnel

#include "pixelram.h"

#include <math.h>

#define W 320
#define H 180

int main(void)
{
    screen_open(W, H, pixel_rgb24, "Tunnel");
    use_palette("endesga_32");

    while (!should_close())
    {
        double t = seconds();
        double cx = W / 2.0 + sin(t * 0.7) * 18;
        double cy = H / 2.0 + cos(t * 0.9) * 10;

        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++)
            {
                double dx = x - cx, dy = y - cy;
                double d = hypot(dx, dy);
                double a = atan2(dy, dx);

                double v = sin(540.0 / (d + 1) + t * 4) + sin(a * 8 + t * 1.2) + 2;

                // v liegt zwischen 0 und 4: eine Stelle zwischen den
                // Paletteneinträgen 0 und 31
                double c = v * 7.75;
                if (c < 0) c = 0;
                if (c > 31) c = 31;
                int i = (int)c, j = i < 31 ? i + 1 : 31;
                double f = c - i;

                uint8_t r0, g0, b0, r1, g1, b1;
                get_palette(i, &r0, &g0, &b0);
                get_palette(j, &r1, &g1, &b1);

                // In der Mitte wird es dunkel.
                double fade = (d - 10) / 30;
                if (fade < 0) fade = 0;
                if (fade > 1) fade = 1;
                fade = fade * fade * (3 - 2 * fade);

                set_pixel_rgb(x, y,
                    (r0 + (r1 - r0) * f) * fade,
                    (g0 + (g1 - g0) * f) * fade,
                    (b0 + (b1 - b0) * f) * fade);
            }

        present();
    }

    screen_close();
    return 0;
}
