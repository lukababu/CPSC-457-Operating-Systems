#!/bin/bash
# Find all .cpp files in the directory

while IFS= read -r line; do echo "$line"; done< <(ls *.cpp)
