#ifndef WORDDICT_H
#define WORDDICT_H

#define MAX_WORD_LENGTH 50
#define MAX_TRIES 6
#define MAX_WORD_LENGTH 50
#define MAX_LINE_LENGTH 100

/**
 * @brief Count lines in file
 * @param filename file with wordlist
 * @return Number of lines
 */
int count_lines_in_file(const char* filename);

/**
 * @brief Get random line from file
 * @param filename full path to wordlist
 * @return Random word
 */
char* get_random_word_from_file(const char* filename);

#endif // WORDDICT_H