#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../src/worddict.h"

static char* create_temp_wordfile(const char** words, int count) {
    char* temp_filename = strdup("/tmp/worddict_test_XXXXXX");
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        free(temp_filename);
        return NULL;
    }

    FILE* f = fdopen(fd, "w");
    if (!f) {
        close(fd);
        free(temp_filename);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        fprintf(f, "%s\n", words[i]);
    }

    fclose(f);
    return temp_filename;
}

#test test_count_lines_in_file
const char* test_words[] = {
    "apple",
    "banana",
    "cherry",
    "pear",
    "strawberry"
};

char* temp_file = create_temp_wordfile(test_words, 5);
ck_assert_ptr_ne(temp_file, NULL);

int line_count = count_lines_in_file(temp_file);
ck_assert_int_eq(line_count, 5);
unlink(temp_file);
free(temp_file);

#test test_get_random_word_from_file
srand(42);
const char* test_words[] = {
    "apple",
    "banana",
    "cherry",
    "pear",
    "strawberry"
};

char* temp_file = create_temp_wordfile(test_words, 5);
ck_assert_ptr_ne(temp_file, NULL);

char* word = get_random_word_from_file(temp_file);
ck_assert_ptr_ne(word, NULL);

int found = 0;
for (int i = 0; i < 5; i++) {
    if (strcmp(word, test_words[i]) == 0) {
        found = 1;
        break;
    }
}
ck_assert_int_eq(found, 1);
free(word);

unlink(temp_file);
free(temp_file);
