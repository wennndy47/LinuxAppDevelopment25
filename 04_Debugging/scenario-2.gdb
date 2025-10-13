file range

set $counter = 0

break range.c:22 if ++$counter >= 28 && $counter <= 35 
    commands 1
    silent
    printf "@@@ start = %d\n", start
    printf "@@@ stop = %d\n", stop
    printf "@@@ step = %d\n", step
    printf "@@@ i = %d\n", i
    continue
end

run -100 100 3 1>/dev/null

quit