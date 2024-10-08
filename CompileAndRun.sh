#!/bin/bash

g++ -std=c++17 -Wall -Wpedantic main.cpp SparseMatrix.cpp -o sparse_matrix
./sparse_matrix
