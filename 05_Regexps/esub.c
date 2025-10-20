#include <stdio.h>
#include <string.h>
#include <regex.h>

#define MAX_GROUPS 10

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s REGEXP SUBSTITUTION STRING\nIt's same as \"echo 'STRING' | sed -E 's/REGEXP/SUBSTITUTION/'\".\n", argv[0]);
        return 1;
    }
    
    const char* regexp = argv[1];
    const char* substitution = argv[2];
    const char* input_string = argv[3];
    int sub_len = strlen(substitution);
    int err;
    
    regex_t regex;
    if ((err = regcomp(&regex, regexp, REG_EXTENDED)) != 0) {
        char error_msg[100];
        regerror(err, &regex, error_msg, sizeof(error_msg));
        fprintf(stderr, "Regex compilation error: %s\n", error_msg);
        return 1;
    }
    
    regmatch_t matches[MAX_GROUPS];
    
    if (regexec(&regex, input_string, MAX_GROUPS, matches, 0) == 0) {
        for (int i = 0; i < strlen(input_string); i++) {
            if (i < matches[0].rm_eo && matches[0].rm_so <= i) {
                for (int j = 0; j < sub_len; j++) {
                    if (j < sub_len - 1 && substitution[j] == '\\' && '0' <= substitution[j+1] && substitution[j+1] <= '9') {
                        int ind = substitution[j+1] - '0';
                        printf("%.*s", matches[ind].rm_eo - matches[ind].rm_so, input_string + matches[ind].rm_so);
                        j++;
                    } else {
                        putchar(substitution[j]);
                    }

                    // backslash handling
                    if (j < sub_len - 1 && substitution[j] == '\\' && substitution[j+1] == '\\') {
                        j++;
                    }
                }
                i = matches[0].rm_eo - 1;
            } else {
                putchar(input_string[i]);
            }
        }
        putchar('\n');
    }
    regfree(&regex);
    return 0;
}
