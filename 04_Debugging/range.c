#include <stdio.h>
#include <stdlib.h>

void print_help() {
    printf("Usage:\n");
    printf("  range N          - prints sequence [0, 1, ... N-1]\n");
    printf("  range M N        - prints sequence [M, M+1, ... N-1]\n");
    printf("  range M N S      - prints sequence [M, M+S, M+2S, ... N-1]\n");
    printf("  range (no args)  - shows this help\n");
    printf("\n");
    printf("Examples:\n");
    printf("  range 5          -> 0 1 2 3 4\n");
    printf("  range 2 7        -> 2 3 4 5 6\n");
    printf("  range 1 10 2     -> 1 3 5 7 9\n");
    printf("  range 10 0 -2    -> 10 8 6 4 2\n");
}

void print_range(int start, int stop, int step) {
    int i = start;
    if (step > 0) {
        for (i = start; i < stop; i += step) {
            printf("%d\n", i);
        }
    } else {
        for (i = start; i > stop; i += step) {
            printf("%d\n", i);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        print_help();
        return 0;
    }
    
    int start, stop, step;
    switch (argc) {
        case 2: 
            stop = atoi(argv[1]);
            start = 0;
            step = 1;
            break;
        case 3:
            start = atoi(argv[1]);
            stop = atoi(argv[2]);
            step = 1;
            break;
        case 4:
            start = atoi(argv[1]);
            stop = atoi(argv[2]);
            step = atoi(argv[3]);
            break;
    }
    
    print_range(start, stop, step);
    return 0;
}
