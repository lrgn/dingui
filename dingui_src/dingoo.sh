#!/bin/bash
mipsel-linux-g++ -O3 -Wall -c Element.cpp
mipsel-linux-g++ -O3 -Wall -c Application.cpp
mipsel-linux-g++ -O3 -Wall -c Interface.cpp
mipsel-linux-g++ -O3 -Wall -c main.cpp
mipsel-linux-g++ -O3 -Wall Element.o Application.o Interface.o main.o -lSDL -lSDL_ttf -lSDL_image -o dingui
rm Element.o
rm Application.o
rm Interface.o
rm main.o
