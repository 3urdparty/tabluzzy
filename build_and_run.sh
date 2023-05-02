#! /bin/sh
cmake -S . -B out/build;
cd out/build; make; 
echo "

Build Sucessfully!
Running tests:
"
ctest;
