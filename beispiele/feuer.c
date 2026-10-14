// Feuer: Jeder Pixel nimmt die Hitze seiner Nachbarn darunter auf und
// kühlt dabei ein wenig ab. Die Farben 0 bis 63 laufen von Schwarz über
// Rot und Gelb bis Weiß.
//
// make PROGRAM=beispiele/feuer

#include "pixelram.h"

#include <stdlib.h>
#include <time.h>

#define W 256
#define H 128

static int clamp(int value, int low, int high)
{
    if (value < low) return low;
    if (value > high) return high;
    return value;
}

int main(void)
{
    screen_open(W, H, pixel_indexed8, "Feuer");
    srand((unsigned int)time(NULL));

    // Schwarz, Rot, Orange, Gelb, Weiß
    for (int i = 0; i < 16; i++)
    {
        set_palette(i, i * 8, 0, 0);
        set_palette(i + 16, i * 8 + 128, i * 8, 0);
        set_palette(i + 32, 255, i * 8 + 128, i * 8);
        set_palette(i + 48, 255, 255, i * 8 + 128);
    }

    while (!should_close())
    {
        // Die untersten beiden Zeilen glühen.
        for (int y = H - 2; y < H; y++)
            for (int x = 10; x < W - 10; x++)
                set_pixel(x, y, 63);

        // Hitze steigt nach oben und flackert ein wenig.
        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++)
            {
                int color = get_pixel(x, y + 1) * 2;
                color += get_pixel(x - 1, y);
                color += get_pixel(x + 1, y);
                color /= 4;

                if (color > 0)
                    color += rand() % 7 - 3;

                set_pixel(x, y, clamp(color, 0, 63));
            }

        present();
    }

    screen_close();
    return 0;
}
