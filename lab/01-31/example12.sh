# create a folder with 10 images and an emprt folder called 'out' before executing
#!/bin/bash

while IFS= read -r line; do convert -resize 100 "$line" "out/$line"; done< <(ls *.jpg)
