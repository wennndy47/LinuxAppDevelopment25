/** @page library 
* Number guesser game
*
* Program for a number guessing game, where the user is supposed to choose a number between 1 and 100
* and the program tries to guess it.
* Supports both roman and arabic numbers.
*/
/** @file guesser.c 
* Implementation
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <libintl.h>
#include <locale.h>
#include "config.h"
#include <string.h>
#include <argp.h>

#define _(STRING) gettext(STRING)

/** Table that contains all roman numerals from 1 to 100.
*/
static const char *ROMAN_NUMERALS[100] = {
    "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X",
    "XI", "XII", "XIII", "XIV", "XV", "XVI", "XVII", "XVIII", "XIX", "XX",
    "XXI", "XXII", "XXIII", "XXIV", "XXV", "XXVI", "XXVII", "XXVIII", "XXIX", "XXX",
    "XXXI", "XXXII", "XXXIII", "XXXIV", "XXXV", "XXXVI", "XXXVII", "XXXVIII", "XXXIX", "XL",
    "XLI", "XLII", "XLIII", "XLIV", "XLV", "XLVI", "XLVII", "XLVIII", "XLIX", "L",
    "LI", "LII", "LIII", "LIV", "LV", "LVI", "LVII", "LVIII", "LIX", "LX",
    "LXI", "LXII", "LXIII", "LXIV", "LXV", "LXVI", "LXVII", "LXVIII", "LXIX", "LXX",
    "LXXI", "LXXII", "LXXIII", "LXXIV", "LXXV", "LXXVI", "LXXVII", "LXXVIII", "LXXIX", "LXXX",
    "LXXXI", "LXXXII", "LXXXIII", "LXXXIV", "LXXXV", "LXXXVI", "LXXXVII", "LXXXVIII", "LXXXIX", "XC",
    "XCI", "XCII", "XCIII", "XCIV", "XCV", "XCVI", "XCVII", "XCVIII", "XCIX", "C"
};

/** Get roman number corresponding to its arabic representation @p num.
*
* @param num an arabic number from 1 to 100
* @return corresponding roman numeral for @p num
*/
const char *arabic_to_roman(int num) {
    return ROMAN_NUMERALS[num - 1];
}

/** Get arabic number corresponding to its roman representation @p roman.
*
* @param roman a roman numeral from I to C
* @return corresponding arabic number for @p roman
*/
int roman_to_arabic(const char *roman) {
    char upper_roman[100];
    int i;
    if (roman == NULL || strlen(roman) == 0) {
        return -1;
    }
    for (i = 0; roman[i] && i < 100 - 1; i++) {
        upper_roman[i] = (roman[i] >= 'a' && roman[i] <= 'z') ? 
                         roman[i] - 'a' + 'A' : roman[i];
    }
    upper_roman[i] = '\0';
    for (i = 0; i < 100; i++) {
        if (strcmp(upper_roman, ROMAN_NUMERALS[i]) == 0) {
            return i + 1;
        }
    }
    return -1;
}

const char *argp_program_version = "guesser 0.1";
const char *argp_program_bug_address = "<me@me.ru>";

struct arguments {
    int use_roman;
};

struct argp_option options[] = {
    {
        "roman",
        'r',
        0,
        0,
        "whether to use roman numerals in the program"
    }
};

static error_t parse_opt (int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
        case 'r':
            arguments->use_roman = 1;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static char doc[] = "A number guessing game";

static struct argp argp = { options, parse_opt, NULL, doc };

/** @page guesser
* Number guessing game
* @section SYNOPSIS
* `guesser` \a use_roman
* @section DESCRIPTION
* Guess a number that was chosen by the user. Use roman numbers if \a use_roman is used.
*
* @copydetails library
*/
int main(int argc, char** argv) {
    struct arguments arguments;
    arguments.use_roman = 0;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    int low = 1;
    int high = 100;
    int guess;
    char answer[10];
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);
    if (arguments.use_roman)
        printf(_("Think of a number between I and C.\n"));
    else
        printf(_("Think of a number between 1 and 100.\n"));

    while (low <= high) {
        guess = (low + high) / 2;

        if (low == high) {
            if (arguments.use_roman) 
                printf(_("Your number: %s\n"), arabic_to_roman(guess));
            else
                printf(_("Your number: %d\n"), guess);
            break;
        }

        while (1) {
            if (arguments.use_roman) 
                printf(_("Is your number bigger than %s? (Yes/No): "), arabic_to_roman(guess));
            else
                printf(_("Is your number bigger than %d? (Yes/No): "), guess);
            
            
            if (fgets(answer, sizeof(answer), stdin) == NULL) {
                printf(_("Input error.\n"));
                return 1;
            }

            answer[strcspn(answer, "\n")] = 0;
            
            if (strcmp(answer, "Yes") == 0) {
                low = guess + 1;
                break;
            } else if (strcmp(answer, "No") == 0) {
                high = guess;
                break;
            } else {
                printf(_("Invalid answer, it should be either yes or no.\n"));
            }
        }
    }

    return 0;
}