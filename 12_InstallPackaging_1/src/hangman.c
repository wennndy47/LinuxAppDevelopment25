/**
 * @mainpage Hangman Game
 * @section synopsis SYNOPSIS
 * hangman [--help] [--version] [--language LANG]
 * 
 * @section description DESCRIPTION
 * Hangman is a game where you try to guess a word.
 * 
 * @section options OPTIONS
 * @par
 * --help, -h
 *     Display this help message and exit.
 * 
 * @par
 * --version, -v
 *     Display version information and exit.
 * 
 * @par
 * --data-dir DATA-DIR, -l DATA-DIR
 *     Set the game wordlist.
 *     Default: wordlist.txt from current directory.
 * 
 * @section files FILES
 * @par
 * /usr/share/hangman/wordlist.txt
 *     English word list
 * @par
 * /usr/share/locale/*\/LC_MESSAGES/hangman.mo
 *     Translation files
 * 
 * @section examples EXAMPLES
 * @par
 * Start the game with default language:
 *     hangman
 * @par
 * Start the game with src/wordlist.txt:
 *     hangman -d src/
 * @par
 * Display help:
 *     hangman --help
 */
#include <config.h>
#include <ctype.h>
#include <getopt.h>
#include <libintl.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <worddict.h>

#define _(STRING) gettext(STRING)
#define LOCALE_PATH "."
#define DEFAULT_DATA_DIR "../game_data"
#define INSTALL_DATA_DIR "/usr/share/hangman"
#define PACKAGE_VERSION "1.0"
#define MAX_WORD_LENGTH 50
#define MAX_TRIES 6

typedef struct {
    const char *data_dir;
    const char *language;
} GameConfig;

/**
 * @brief Display version information
 */
static void show_version(void) {
    printf(_("Hangman Game version %s\n"), PACKAGE_VERSION);
}

/**
 * @brief Display help information
 */
static void show_help(void) {
    printf(_("Usage: hangman [OPTION]\n"));
    printf(_("\n"));
    printf(_("Word guessing game.\n"));
    printf(_("\n"));
    printf(_("Options:\n"));
    printf(_("  -h, --help           display this help and exit\n"));
    printf(_("  -v, --version        display version information and exit\n"));
    printf(_("  -d, --data-dit DATA_DIR  set directory containing word lists (en, ru)\n"));
    printf(_("\n"));
    printf(_("Environment variables:\n"));
    printf(_("  DATA_DIR                 determines directory with word lists\n"));
    printf(_("\n"));
    printf(_("Examples:\n"));
    printf(_("  hangman              start game with default language\n"));
    printf(_("\n"));
    printf(_("Report bugs to: <me@me.com>\n"));
}

#define MAX_WORD_LENGTH 50
#define MAX_LINE_LENGTH 100

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

/**
 * @brief Get game data directory
 * @param cmdline_dir directory passed in command line (if set)
 * @return Found data directory
 */
static char* get_data_dir(const char* cmdline_dir) {
    char* data_dir = NULL;

    // cmd first
    if (cmdline_dir != NULL) {
        data_dir = strdup(cmdline_dir);
        if (data_dir && access(data_dir, R_OK) == 0) {
            return data_dir;
        }
        if (data_dir) {
            free(data_dir);
        }
    }
    // then env
    char* env_dir = getenv("HANGMAN_DATA_DIR");
    if (env_dir != NULL) {
        data_dir = strdup(env_dir);
        if (data_dir && access(data_dir, R_OK) == 0) {
            return data_dir;
        }
        if (data_dir) {
            free(data_dir);
        }
    }
    // then default
    data_dir = strdup(DEFAULT_DATA_DIR);
    if (data_dir && access(data_dir, R_OK) == 0) {
        return data_dir;
    }
    if (data_dir) {
        free(data_dir);
    }
    // then /usr/share
    data_dir = strdup(INSTALL_DATA_DIR);
    if (data_dir && access(data_dir, R_OK) == 0) {
        return data_dir;
    }
    return strdup(DEFAULT_DATA_DIR);
}

/**
 * @brief Get full wordlist path
 * @param data_dir Directory with wordlists
 * @return Full path to wordlist
 */
static char* get_wordlist_path(const char* data_dir) {
    char* filename = "wordlist.txt";
    char* full_path = NULL;
    full_path = malloc(strlen(data_dir) + strlen(filename) + 2);
    if (!full_path) return NULL;
    sprintf(full_path, "%s/%s", data_dir, filename);
    return full_path;
}

/**
 * @brief Get word from the wordlist
 * @param config Config with language code and filepath to wordlist
 * @return Filename for the word list
 */
static char* get_word_with_config(const GameConfig* config) {
    char* word = NULL;
    char* data_dir = NULL;
    char* wordlist_path = NULL;

    data_dir = get_data_dir(config->data_dir);
    if (!data_dir) {
        fprintf(stderr, _("Failed to determine data directory\n"));
        return NULL;
    }
    struct stat statbuf;
    if (stat(data_dir, &statbuf) != 0) {
        fprintf(stderr, _("Data directory does not exist: %s\n"), data_dir);
        return NULL;
    }
    if (!S_ISDIR(statbuf.st_mode)) {
        fprintf(stderr, _("Data directory does not exist: %s\n"), data_dir);
        return NULL;
    }

    wordlist_path = get_wordlist_path(data_dir);
    if (!wordlist_path) {
        fprintf(stderr, _("Failed to construct wordlist path\n"));
        free(data_dir);
        return NULL;
    }
    word = get_random_word_from_file(wordlist_path);
    if (!word) {
        fprintf(stderr, _("Failed to get a word from wordlist.\n"));
        free(data_dir);
        return NULL;
    }
    free(data_dir);
    free(wordlist_path);
    return word;
}

typedef struct {
    char secret_word[MAX_WORD_LENGTH];
    char guessed_letters[MAX_WORD_LENGTH];
    char current_guess[MAX_WORD_LENGTH];
    int tries_left;
    int game_over;
    int won;
} HangmanGame;

/**
 * @brief Create a game
 * @return Game
 */
HangmanGame* create_game() {
    HangmanGame* game = malloc(sizeof(HangmanGame));
    if (game) {
        memset(game, 0, sizeof(HangmanGame));
        game->tries_left = MAX_TRIES;
        game->game_over = 0;
        game->won = 0;
    }
    return game;
}

/**
 * @brief Clean up the game
 * @param game game struct
 */
void cleanup(HangmanGame* game) {
    free(game);
}

/**
 * @brief Load word into the game
 * @param game game struct
 * @param word secret word
 * @return Exit code
 */
int load_word(HangmanGame* game, const char* word) {
    strcpy(game->secret_word, word);
    game->secret_word[strlen(game->secret_word) - 1] = '\0';
    memset(game->guessed_letters, 0, sizeof(game->guessed_letters));
    for (int i = 0; i < strlen(word) - 1; i++) {
        if (word[i] == ' ') {
            game->current_guess[i] = ' ';
        } else {
            game->current_guess[i] = '_';
        }
    }
    game->current_guess[strlen(word)] = '\0';
    return 1;
}

/**
 * @brief Checks if letter is present in the word
 * @param game game struct
 * @param letter guessed letter
 * @return Whether the letter is present in the word
 */
int guess_letter(HangmanGame* game, char letter) {    
    letter = tolower(letter);

    // this letter was guessed already
    for (int i = 0; i < strlen(game->guessed_letters); i++) {
        if (game->guessed_letters[i] == letter) return 0;
    }
    int len = strlen(game->guessed_letters);
    game->guessed_letters[len] = letter;
    game->guessed_letters[len + 1] = '\0';

    int correct_guess = 0;
    for (int i = 0; i < strlen(game->secret_word); i++) {
        if (tolower(game->secret_word[i]) == letter) {
            game->current_guess[i] = game->secret_word[i];
            correct_guess = 1;
        }
    }
    if (!correct_guess) game->tries_left--;

    if (strcmp(game->current_guess, game->secret_word) == 0) {
        game->won = 1;
        game->game_over = 1;
    } else if (game->tries_left <= 0) {
        game->won = 0;
        game->game_over = 1;
    }
    return correct_guess;
}

/**
 * @brief Check if game is over already
 * @param game game struct
 * @return whether game is over
 */
int is_game_over(const HangmanGame* game) {
    return game ? game->game_over : 1;
}

/**
 * @brief Check if game is won
 * @param game game struct
 * @return whether game is won
 */
int is_win(const HangmanGame* game) {
    return game ? game->won : 0;
}

/**
 * @brief Draw the hangman
 * @param game game struct
 */
void hangman_draw(const HangmanGame* game) {
    if (!game) return;

    printf("\n");
    printf(_("Word: %s\n"), game->current_guess);
    printf(_("Tries left: %d\n"), game->tries_left);
    printf(_("Guessed letters: %s\n"), game->guessed_letters);
    printf("\n");
    switch (game->tries_left) {
        case 6:
            printf(_("  _______\n"));
            printf(_("  |     |\n"));
            printf(_("  |\n"));
            printf(_("  |\n"));
            printf(_("  |\n"));
            printf(_("  |\n"));
            printf(_("__|________\n"));
            break;
        case 5:
            printf(_("  _______\n"));
            printf(_("  |     |\n"));
            printf(_("  |     O\n"));
            printf(_("  |\n"));
            printf(_("  |\n"));
            printf(_("  |\n"));
            printf(_("__|________\n"));
            break;
        case 4:
            printf(_("  _______\n"));
            printf(_("  |     |\n"));
            printf(_("  |     O\n"));
            printf(_("  |     |\n"));
            printf(_("  |\n"));
            printf(_("  |\n"));
            printf(_("__|________\n"));
            break;
        case 3:
            printf(_("  _______\n"));
            printf(_("  |     |\n"));
            printf(_("  |     O\n"));
            printf(_("  |    /|\n"));
            printf(_("  |\n"));
            printf(_("  |\n"));
            printf(_("__|________\n"));
            break;
        case 2:
            printf(_("  _______\n"));
            printf(_("  |     |\n"));
            printf(_("  |     O\n"));
            printf(_("  |    /|\\\n"));
            printf(_("  |\n"));
            printf(_("  |\n"));
            printf(_("__|________\n"));
            break;
        case 1:
            printf(_("  _______\n"));
            printf(_("  |     |\n"));
            printf(_("  |     O\n"));
            printf(_("  |    /|\\\n"));
            printf(_("  |    /\n"));
            printf(_("  |\n"));
            printf(_("__|________\n"));
            break;
        case 0:
            printf(_("  _______\n"));
            printf(_("  |     |\n"));
            printf(_("  |     O\n"));
            printf(_("  |    /|\\\n"));
            printf(_("  |    / \\\n"));
            printf(_("  |\n"));
            printf(_("__|________\n"));
            break;
    }
    printf("\n");
}

/**
 * @brief The game itself
 * @param config Game config
 * @return Exit code
 */
static int game(const GameConfig* config) {
    srand(time(NULL));
    printf(_("Welcome to Hangman!\n"));
    printf(_("===================\n\n"));

    char* word = get_word_with_config(config);
    if (!word) {
        return 1;
    }

    HangmanGame* game = create_game();
    if (!game) {
        fprintf(stderr, _("Failed to create game\n"));
        return 1;
    }

    if (!load_word(game, word)) {
        fprintf(stderr, _("Failed to load word\n"));
        cleanup(game);
        return 1;
    }

    while (!is_game_over(game)) {
        hangman_draw(game);
        printf(_("\nGuess a letter: "));
        char input[10];
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        if (strlen(input) == 0) continue;
        char letter = input[0];
        if (!isalpha((unsigned char)letter)) {
            printf(_("Please enter a valid letter\n"));
            continue;
        }
        guess_letter(game, letter);
        if (is_win(game)) {
            printf(_("\nCongratulations! You won!\n"));
            printf(_("The word was: %s\n"), word);
            cleanup(game);
            return 0;
        }
    }
    hangman_draw(game);
    if (is_win(game)) {
        printf(_("\nCongratulations! You won!\n"));
        printf(_("The word was: %s\n"), word);
    } else {
        printf(_("\nGame over! You lost.\n"));
        printf(_("The word was: %s\n"), word);
    }
    cleanup(game);
    return 0;
}

/**
 * @brief Main entrypoint
 * @param argc argument count
 * @param argv list of arguments
 * @return Exit status
 */
int main(int argc, char** argv) {
    GameConfig config = {0};

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"data-dir", required_argument, 0, 'd'},
        {0, 0, 0, 0}
    };
    
    int opt;
    int option_index = 0;
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALE_PATH);
    textdomain(PACKAGE);
    while ((opt = getopt_long(argc, argv, "hvd:", 
                  long_options, &option_index)) != -1) {
                switch (opt) {
            case 'h':
                show_help();
                return 0;
            case 'v':
                show_version();
                return 0;
            case 'd':
                config.data_dir = optarg;
                break;
            default:
                fprintf(stderr, _("Try 'hangman --help' for more information.\n"));
                return 1;
        }
    }

    return game(&config);
}