#!/bin/bash

CWD=$PWD
COMMONS="so-commons-library"
echo "\nPath: ${CWD}"
cd $CD
echo "\nPath: ${PWD}"
echo "\nInstalando commons..."
#git clone "https://github.com/sisoputnfrba/${COMMONS}.git" $COMMONS
cd $COMMONS
echo "\nPath: ${PWD}"
#sudo make install
cd $PWD
echo "\nPath: ${CWD}"