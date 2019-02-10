#!/bin/bash

echo Compiling main.cpp...
g++ -Wall -Wextra -c -g main.cpp

echo Compiling rmvlib.cpp
g++ -Wall -Wextra -c -g rmvlib.cpp

echo Linking everything together...
g++ -o wf rmvlib.o main.o -lstdc++fs

echo Bye Bye!
