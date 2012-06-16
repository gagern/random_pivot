#!/bin/sh

rbase=$(ls -d1 "/cygdrive/c/Program Files/R"/R-*/ | tail -n1)
set -ex

x86_64-w64-mingw32-g++.exe \
 -I "${rbase}/include" \
 -O2 -Wall \
 -c random_pivot.cc

x86_64-w64-mingw32-g++.exe \
 -shared -s \
 -static-libgcc -static-libstdc++ \
 -o random_pivot.dll \
 random_pivot-win64.def random_pivot.o \
 -L "${rbase}/bin/x64" -lR
