#!/bin/bash

# Bluring image
for file in $*; do
  convert "$file" -blur 0x32 "blur-${file}"
done
