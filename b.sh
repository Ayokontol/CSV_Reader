#!/bin/bash
for file in ./test/*.csv
do
echo $file
./csvreader.exe $file
done

