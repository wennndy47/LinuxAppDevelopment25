file range

break range.c:22 if i % 5 == 0
    commands 1
    silent
    printf "@@@ start = %d\n", start
    printf "@@@ stop = %d\n", stop
    printf "@@@ step = %d\n", step
    printf "@@@ i = %d\n", i
    continue
end

run 1 12 1>/dev/null

quit