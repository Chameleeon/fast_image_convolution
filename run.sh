#!/bin/bash

premake5 gmake2 > /dev/null 2>&1
make config=release > /dev/null 2>&1


./bin/Release-linux-x86_64/main/main

make clean > /dev/null 2>&1


premake5 gmake2 > /dev/null 2>&1

make config=releasecsimd > /dev/null 2>&1


./bin/ReleaseCSIMD-linux-x86_64/main/main

# Clean up
make clean > /dev/null 2>&1 > /dev/null 2>&1

