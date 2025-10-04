#!/bin/dash

tput setab 4  
delay=${1:-0.1}

lines=$(tput lines)
arr_file=$(mktemp)
coord_file=$(mktemp)
input=$(cat)
Y=0
echo "$input" | while IFS= read -r line; do
	X=0
	echo "$line" | sed 's/./&\n/g' | while IFS= read -r char; do
		if [ -n "$char" ] && [ "$char" != " " ]; then
            echo "$char:$X:$Y" >> "$arr_file"
        fi
        X=$((X + 1))
	done
	Y=$(($Y + 1))
done

char_count=$(wc -l < "$arr_file")
seq 0 $((char_count - 1)) | shuf > "$coord_file"

tput clear
tput setab 4  
tput setaf 7
tput home

while IFS= read -r i; do
    line_num=$((i + 1))
    line=$(sed -n "${line_num}p" "$arr_file" 2>/dev/null)
    
    if [ -n "$line" ]; then
        char=$(echo "$line" | cut -d: -f1)
        x=$(echo "$line" | cut -d: -f2)
        y=$(echo "$line" | cut -d: -f3)
        
        tput cup $y $x
        sleep $delay
        printf "%s" "$char"
    fi
done < "$coord_file"

rm -f "$arr_file" "$coord_file"
tput cup $lines 0
tput sgr0