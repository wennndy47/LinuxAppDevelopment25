#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "worddict.h"


/**
 * @brief Count lines in file
 * @param filename file with wordlist
 * @return Number of lines
 */
int count_lines_in_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    int ch = 0;
    int lines = 0;
    while(!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n') lines++;
    }    
    fclose(file);
    return lines;
}

/**
 * @brief Get random line from file
 * @param filename full path to wordlist
 * @return Random word
 */
char* get_random_word_from_file(const char* filename) {
    if (!filename) {
        return NULL;
    }
    FILE* file = fopen(filename, "r");    
    if (!file) return NULL;
    
    int total_lines = count_lines_in_file(filename);
    if (total_lines == 0) {
        fclose(file);
        return NULL;
    }
    
    int target_line = rand() % total_lines;
    int current_line = 0;
    rewind(file);
    
    char buffer[MAX_LINE_LENGTH];
    char* result = NULL;
    
    while (fgets(buffer, sizeof(buffer), file)) {       
        if (current_line == target_line) {
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
            result = malloc(len + 1);
            if (result) {
                strcpy(result, buffer);
            }
            break;
        }
        current_line++;
    }
    fclose(file);
    return result;
}
