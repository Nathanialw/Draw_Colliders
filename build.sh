#!/bin/bash

#build the project

mkdir build
cd build
cmake ..
make

#copy the assets to the build directory
cp -R ../assets .

# run
./Box2D_Colliders