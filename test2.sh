#!/bin/sh

echo "TEST2"
./main "filesystem" 1 
./main "filesystem" 5 "bigfile.txt"
./main "filesystem" 5 "file11.txt"
./main "filesystem" 5 "file12.txt"
./main "filesystem" 5 "file13.txt"
./main "filesystem" 5 "file14.txt"
./main "filesystem" 3
./main "filesystem" 4
rm "file14.txt"
./main "filesystem" 6 "file14.txt"
./main "filesystem" 3
./main "filesystem" 4
./main "filesystem" 2
