#!/bin/bash
#TODO: Create a MakeFile
g++ histogram.cpp -o hist `pkg-config --libs --cflags opencv`

echo "====================================="
echo "Compiled... Try hist image_train.txt"
echo "====================================="
