# PixelRAM Starter

A starter project for [PixelRAM](https://github.com/specht/pixelram), used by
the Hackschule tutorial »Echtzeitgrafik mit C«.

The project begins with a single pixel in `main.c`. The Makefile downloads the
pinned PixelRAM dependency into a versioned hidden directory such as
`.pixelram/v0.1.22/` on the first build.

## Build

In Hackschule Workspace:

```sh
make
```

This creates `main.html`. Any other program in the folder builds the same way,
`make PROGRAM=lichtspur` turns `lichtspur.c` into `lichtspur.html`.

## Examples

`beispiele/` holds finished programs to run and change:

```sh
make PROGRAM=beispiele/feuer    # fire from the heat of the neighbouring pixels
make PROGRAM=beispiele/tunnel   # flight through a tunnel from distance and angle
make PROGRAM=beispiele/julia    # a slowly changing Julia set
make PROGRAM=beispiele/magie    # palette cycling: only the colours move
make PROGRAM=beispiele/sterne   # 400 stars flying towards you in 3D
```

## A whole game

```sh
make crystal
```

Clones the PixelRAM repository at the pinned version and builds its port of
Crystal Caves (1991), whose shareware episode comes with the OpenCrystalCaves
source. The result is `spiele/crystal.html`; arrow keys walk, Z jumps, X
shoots. The other ports have their own descriptions under `ports/` in the
PixelRAM repository.

## Cleaning up

`make clean` removes the build files and keeps the downloaded PixelRAM files.
`make distclean` also removes the downloaded PixelRAM copy and `spiele/`.

To update PixelRAM, change only `PIXELRAM_VERSION` in the Makefile. Because the
version is part of the hidden cache path, the next `make` downloads that
release.
