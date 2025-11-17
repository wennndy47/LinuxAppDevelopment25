#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <libintl.h>
#include <locale.h>
#include "config.h"
#include <string.h>

#define _(STRING) gettext(STRING)

int main() {
    int low = 1;
    int high = 100;
    int guess;
    char answer[10];
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);
    printf(_("Think of a number between 1 and 100.\n"));

    while (low <= high) {
        guess = (low + high) / 2;

        if (low == high) {
            printf(_("Your number: %d\n", guess));
            break;
        }

        while (1) {
            printf(_("Is your number bigger than %d? (Yes/No): ", guess));
            
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