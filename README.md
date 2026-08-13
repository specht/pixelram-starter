# PixelRAM Starter

A minimal starter project for [PixelRAM](https://github.com/specht/pixelram).

The project begins with a single pixel in `main.c`. The Makefile downloads the
pinned PixelRAM dependency into a versioned hidden directory such as
`.pixelram/v0.1.19/` on the first build.

## Build

In Hackschule Workspace:

```sh
make
```

This creates:

```text
main.html
```

Normal cleanup keeps the downloaded PixelRAM files:

```sh
make clean
```

To remove build files and the downloaded PixelRAM copy:

```sh
make distclean
```

PixelRAM is currently pinned to `v0.1.19`. To update, change only
`PIXELRAM_VERSION` in the Makefile. Because the version is part of the hidden
cache path, the next `make` automatically downloads that release.
