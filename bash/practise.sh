#!/bin/bash 

if [ $# -eq 0 ]; then
    echo "enter dir"
    exit 1
fi

dir="$1"

if [ ! -d "$dir" ]; then
    echo "no such dir found"
    exit 1
fi


largest=$(find "$dir" \( -name "*.txt" \) -type f -exec du -k {} + | sort -nr | head -n 1 )
files_in_dir=$(find "$dir" -type f )
file_content=$(echo "$largest" | wc -l)
patt="file"
count=0

for f in "$dir"/*; do
    ext="${f##*.}"
    new_name="$patt"_"$count".${ext}
    if  mv "$f" "$dir/$new_name" ; then   
        echo "name changed from $f to $new_name"
    else 
        echo "rename failed"
    fi
   ((count++))
done




echo "$largest"
echo "$file_content"
echo "file in dir : $files_in_dir"

