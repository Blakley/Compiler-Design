#!/bin/bash

# locate files with the specified extensions and delete them
find . -type f \( -name "*.inorder" -o -name "*.postorder" -o -name "*.preorder" \) -exec rm -f {} \;
