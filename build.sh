#!/bin/sh
cc -DTEXTURES src/*.c -Wno-undefined-inline -F/Library/Frameworks -framework SDL2 -framework SDL2_image -o 3d.arm64