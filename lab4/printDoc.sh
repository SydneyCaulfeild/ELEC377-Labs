#!/bin/sh
# Find and remove comments from c files and header files.
for file in `find -name '*.c' -or -name '*.h'`; do
    echo "========================================================================="
    sed -e '/\/\/+/,/\/\/-/p' -e 'd' $file | sed -e 's/\/\/[+-]\{0,1\}//'  
done

