#include <stdio.h>
#include <stdlib.h>
#include "rhash.h"
#include <string.h>
#include <ctype.h>

#define READLINE_AVAILABLE

#ifdef READLINE_AVAILABLE
#include <readline/readline.h>
#include <readline/history.h>
#endif

int is_string (const char* quoted_string) {
    return quoted_string[0] == '"';
}

char* extract_string (const char* quoted_string) {
    if (strlen(quoted_string) <= 2) return NULL;
    const char* start_quote = strchr(quoted_string, 34);
    if (start_quote == NULL) return NULL;
    const char* end_quote = strchr(start_quote + 1,  34);
    if (end_quote == NULL) return NULL;
    size_t len = end_quote - start_quote - 1;
    if (len <= 0) return strdup("");
    char* content = malloc(len + 1);
    if (!content) return NULL;
    strncpy(content, quoted_string + 1, len);
    content[len] = '\0';
    return content;
}

int get_alg_id (const char* alg_name) {
    if (strcasecmp(alg_name, "MD5") == 0) return RHASH_MD5;
    if (strcasecmp(alg_name, "SHA1") == 0) return RHASH_SHA1;
    if (strcasecmp(alg_name, "TTH") == 0) return RHASH_TTH;
    return -1;
}

#define UNKNOWN_COMMAND 100
#define HASH_FAILED 101

int hash_string (const char* alg_name, char* input) {
    rhash_library_init();
    int alg_id = get_alg_id(alg_name);
    if (alg_id == -1) return UNKNOWN_COMMAND;

    unsigned char digest[64];
    char output[130];
    int res = rhash_msg(alg_id, input, strlen(input), digest);
    if (res < 0) return HASH_FAILED;

    int output_mode = isupper((unsigned char)alg_name[0]) ? RHPR_HEX : RHPR_BASE64;
    rhash_print_bytes(output, digest, rhash_get_digest_size(alg_id), output_mode);
    fprintf(stdout, "%s\n", output);
    return 0;
}

int hash_file(const char* alg_name, const char* filename) {
    rhash_library_init();
    int alg_id = get_alg_id(alg_name);
    if (!alg_id) return UNKNOWN_COMMAND;
    unsigned char digest[64];
    char output[130];
    int res = rhash_file(RHASH_TTH, filename, digest);
    if (res < 0) return HASH_FAILED;
    int output_mode = isupper((unsigned char)alg_name[0]) ? RHPR_HEX : RHPR_BASE64;
    rhash_print_bytes(output, digest, rhash_get_digest_size(alg_id), output_mode);
    printf("%s\n", output);
    return 0;
}

int main (int argc, char** argv) {
    if (argc > 1) {
        fprintf(stderr, "Incorrect usage.\n");
        return 1;
    }
#ifdef READLINE_AVAILABLE
    char* line;
    
    while ((line = readline("> ")) != NULL) {
        if (strlen(line) == 0) {
            free(line);
            continue;
        }
        
        add_history(line);
#else
    printf("> ");
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    
    while ((read = getline(&line, &len, stdin)) != -1) {
        if (read > 0 && line[read - 1] == '\n') {
            line[read - 1] = '\0';
            read--;
        }
        
        if (read == 0) {
            printf("> ");
            continue;
        }
#endif
        
        char* token = strtok(line, " ");
        if (token == NULL) {
#ifndef READLINE_AVAILABLE
            printf("> ");
#endif
            continue;
        }
        
        char* alg_name = token;
        char* input = strtok(NULL, " ");
        
        if (input == NULL) {
            fprintf(stderr, "Second parameter is missing.\n");
        } else {
            if (is_string(input)) {
                char* str_content = extract_string(input);
                if (str_content == NULL) {
                    fprintf(stderr, "Corrupted string provided as input: %s.\n", input);
                    free(line);
                    return 1;
                }
                int res = hash_string(alg_name, str_content);
                if (res == UNKNOWN_COMMAND) {
                    free(str_content);
                    free(line);
                    fprintf(stderr, "Unknown command provided.\n");
                    return 1;
                } else if (res == HASH_FAILED) {
                    free(str_content);
                    free(line);
                    fprintf(stderr, "Message digest calculation error\n");
                    return 1;
                }
                free(str_content);
            } else {
                hash_file(alg_name, input);
            }
        }
#ifdef READLINE_AVAILABLE
        free(line);
#else
        printf("> ");
#endif
    }
#ifndef READLINE_AVAILABLE
    free(line);
#endif
    return 0;
}