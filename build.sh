#!/bin/bash
mkdir bin

echo Compiling main.cpp...
g++ -Wall -Wextra -c -g main.cpp

echo Compiling rmvlib.cpp
g++ -Wall -Wextra -c -g rmvlib.cpp

echo Linking everything together...
g++ -o bin/wf rmvlib.o main.o -lstdc++fs

rm rmvlib.o
rm main.o
echo Bye Bye!
