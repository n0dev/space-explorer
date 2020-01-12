#!/bin/bash

mkdir .build
cd .build

cmake ..
make

cp space-explorer ../bin/
