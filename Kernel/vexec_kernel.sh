#!/bin/bash

make

valgrind --leak-check=full ./Kernel.out
