#!/bin/sh

clang -Oz -fno-builtin -Wall -Wextra -Wswitch-enum --target=wasm32 --no-standard-libraries -Wl,--export=update -Wl,--export=getScreenImageData -Wl,--export=getScreenWidth -Wl,--export=getScreenHeight -Wl,--export=init -Wl,--no-entry -Wl,--allow-undefined  -o main.wasm main.c
