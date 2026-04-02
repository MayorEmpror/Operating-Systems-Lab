#!/bin/bash

if [ $# -eq 0 ]; then 
    echo "params not provided"
    exit 1 
fi

dir="$1"
if [ ! -d "$dir" ]; then
    echo "not such dir exists"
    exit 1 
fi
fonund_files=$(find "$dir" \( -name "*.txt" \))
count=$(echo "$fonund_files" | wc -l )

echo "number of text files present  $count"
