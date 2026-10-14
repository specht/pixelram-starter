// Sternenflug: 400 Sterne fliegen auf dich zu. Ein Stern bei (x, y, z)
// erscheint auf dem Bildschirm bei x / z und y / z: Je näher er kommt
// (z wird kleiner), desto weiter außen und desto heller ist er.
//
// make PROGRAM=beispiele/sterne

#include "pixelram.h"

#include <stdlib.h>

#define W 320
#define H 180
#define STARS 400

typedef struct
{
    double x, y, z;
} Star;

static double random_between(double low, double high)
{
    return low + (high - low) * rand() / (double)RAND_MAX;
}

static void new_star(Star *s, double z)
{
    s->x = random_between(-1, 1);
    s->y = random_between(-1, 1);
    s->z = z;
}

int main(void)
{
    screen_open(W, H, pixel_indexed8, "Sterne");

    // Farben 0 bis 15: von Schwarz bis Weiß
    for (int i = 0; i < 16; i++)
        set_palette(i, i * 15, i * 15, i * 17);

    Star stars[STARS];
    for (int i = 0; i < STARS; i++)
        new_star(&stars[i], random_between(0.05, 1));

    double last = seconds();
    while (!should_close())
    {
        double now = seconds();
        double dt = now - last;
        last = now;

        clear(0);
        for (int i = 0; i < STARS; i++)
        {
            Star *s = &stars[i];
            s->z -= dt * 0.3;
            if (s->z < 0.02)
                new_star(s, 1);

            int sx = (int)(W / 2 + s->x / s->z * H / 2);
            int sy = (int)(H / 2 + s->y / s->z * H / 2);
            if (sx < 0 || sx >= W || sy < 0 || sy >= H)
            {
                new_star(s, 1);
                continue;
            }

            set_pixel(sx, sy, (int)((1 - s->z) * 15));
        }

        present();
    }

    screen_close();
    return 0;
}
