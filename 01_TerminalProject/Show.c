#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#define DX 7
#define DY 3

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

struct FileInfo {
    int lines_count;
    char** lines;
};

void read_file(char* file_name, struct FileInfo* file) {
    FILE *f = fopen(file_name, "r");
    int lines_count = 0;
    while (!feof(f)) {
        char c = fgetc(f);
        if (c == '\n') {
            ++lines_count;
        }
    }
    fseek(f, 0, SEEK_SET);
    char** lines = malloc(lines_count * sizeof(char*));
    size_t len = 0;
    for (int i = 0; i < lines_count; ++i) {
        getline(lines + i, &len, f);
    }
    fclose(f);
    file->lines = lines;
    file->lines_count = lines_count;
}

void main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Incorrect number of arguments passed\n");
        return;
    }
    
    struct FileInfo file;
    read_file(argv[1], &file);
    char** file_lines = file.lines;
    int lines_count = file.lines_count;
    
    initscr();
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
    setlocale(LC_ALL, "");
    
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    WINDOW* frame = newwin(max_y - 2 * DY, max_x - 2 * DX, DY, DX);
    box(frame, 0, 0);
    mvwaddstr(frame, 0, (int)((max_x - 2 * DX - strlen(argv[1])) / 2), argv[1]); // frame with filename
    wrefresh(frame);
    
    int window_height = max_y - 2 * DY - 2;
    int window_width = max_x - 2 * DX - 2;
    WINDOW* win = newwin(window_height, window_width, DY + 1, DX + 1);
    keypad(win, FALSE);
    scrollok(win, FALSE);
    
    int c;
    int start = 0;
    int end = window_height;
    if (end > lines_count) {
        end = lines_count;
    }
    
    for (int i = start; i < end; i++) {
        int len = strlen(file_lines[i]);
        if (len > window_width) {
            file_lines[i][window_width] = '\0';
        }
        wprintw(win, "%s", file_lines[i]);
    }
    wmove(win, 0, 0);
    wrefresh(win);
    
    while((c = wgetch(win)) != 27) {
        if (c == ' ') {
            werase(win);
            if (start < lines_count - window_height) {
                start++;
                end++;
            }
            for (int i = start; i < end; i++) {
                int len = strlen(file_lines[i]);
                if (len > window_width) {
                    file_lines[i][window_width] = '\0';
                }
                wprintw(win, "%s", file_lines[i]);
            }
            wmove(win, 0, 0);
            wrefresh(win);
        }
    }
    delwin(win);
    delwin(frame);
    endwin();
}
