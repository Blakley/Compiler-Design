#!/bin/bash

# locate files with the specified extensions and delete them
find . -type f \( -name "*.file1" -o -name "*.file2" -o -name "*.file3" \) -exec rm -f {} \;
