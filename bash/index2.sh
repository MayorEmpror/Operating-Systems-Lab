#!/bin/bash

if [ $# -eq 0 ]; then 
    echo "params not provided"
    exit 1
fi

file="deleted_files.log"
dir="$1"

if [ ! -d "$dir" ]; then
    echo "dir not valid"
    exit 1
fi

files_found=$(find "$dir" \( -name "*.tmp" -o -name "*.log" \) -mtime -7)

# Count files BEFORE deleting
num_files_del=$(echo "$files_found" | wc -l)

# Delete files
find "$dir" \( -name "*.tmp" -o -name "*.log" \) -mtime -7 -delete

# Log files
echo "$files_found" >> "$file"

# Output
cat "$file"
echo "Deleted $num_files_del files"