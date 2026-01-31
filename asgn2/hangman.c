#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hangman_strings.h"

#define MAX_SECRET_LENGTH 256
#define MAX_WRONG_GUESSES 6

// TODO: Replace these with the EXACT strings used by the reference binary
// If your starter includes error macros in hangman_strings.h, use those instead.
#define ERR_MISSING_SECRET "The secret phrase is missing.\n"
#define ERR_SECRET_TOO_LONG "The secret phrase is over 256 characters.\n"
// This one is almost certainly NOT correct for your class unless the starter says so.
#define ERR_INVALID_SECRET "The secret phrase has an invalid character.\n"

static int is_lowercase_letter(char c) {
    return c >= 'a' && c <= 'z';
}

static int is_uppercase_letter(char c) {
    return c >= 'A' && c <= 'Z';
}

static char to_lower_ascii(char c) {
    if (is_uppercase_letter(c)) return (char)(c - 'A' + 'a');
    return c;
}

static int is_special(char c) {
    return (c == ' ' || c == '-' || c == '\'');
}

static int string_contains_character(const char *str, char c) {
    if (!str) return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == c) return 1;
    }
    return 0;
}

// Valid secret: non-NULL, length <= 256, chars in [a-z] plus space/'/-
static int is_valid_secret(const char *secret) {
    if (!secret) return 0;

    size_t n = strlen(secret);
    if (n > MAX_SECRET_LENGTH) return 0;

    for (size_t i = 0; i < n; i++) {
        char c = secret[i];
        if (!is_lowercase_letter(c) && !is_special(c)) {
            return 0;
        }
    }
    return 1;
}

static char prompt_for_and_read_character(void) {
    printf("%s", MSG_PROMPT);

    int ch;
    do {
        ch = getchar();
        if (ch == EOF) exit(1);
    } while (ch == '\n');

    return (char)ch;
}

static void print_state(int wrong, const char *display, const char *eliminated) {
    printf("%s", arts[wrong]);
    printf("%s%s\n", MSG_PHRASE, display);
    printf("%s%s\n", MSG_ELIMINATED, eliminated);
}

static void insert_sorted(char *eliminated, char guess) {
    int len = (int)strlen(eliminated);

    int pos = 0;
    while (pos < len && eliminated[pos] < guess) pos++;

    for (int i = len; i >= pos; i--) {
        eliminated[i + 1] = eliminated[i];
    }
    eliminated[pos] = guess;
}

void run_hangman(const char *secret) {
    int secret_len = (int)strlen(secret);

    char display[MAX_SECRET_LENGTH + 1];
    char eliminated[27];
    eliminated[0] = '\0';

    int wrong = 0;
    int remaining = 0;

    // Build initial display with hints for special chars
    for (int i = 0; i < secret_len; i++) {
        char c = secret[i];
        if (is_lowercase_letter(c)) {
            display[i] = '_';
            remaining++;
        } else {
            display[i] = c; // space, hyphen, apostrophe shown
        }
    }
    display[secret_len] = '\0';

    // If only special chars, win immediately (but still print initial state)
    if (remaining == 0) {
        print_state(0, display, eliminated);
        printf("%s%s\n", MSG_WIN, secret);
        return;
    }

    while (1) {
        // Always print board at start of each "turn"
        print_state(wrong, display, eliminated);

        if (wrong >= MAX_WRONG_GUESSES) {
            printf("%s%s\n", MSG_LOSE, secret);
            return;
        }
        if (remaining == 0) {
            printf("%s%s\n", MSG_WIN, secret);
            return;
        }

        // Read guesses until we get a NEW letter guess
        char guess;
        while (1) {
            guess = prompt_for_and_read_character();
            guess = to_lower_ascii(guess);

            // Only letters count as guesses
            if (!is_lowercase_letter(guess)) {
                continue;
            }

            // If already guessed (either correct or eliminated),
            // we re-prompt WITHOUT reprinting board (matches spec)
            if (string_contains_character(display, guess)) continue;
            if (string_contains_character(eliminated, guess)) continue;

            break;
        }

        if (string_contains_character(secret, guess)) {
            for (int i = 0; i < secret_len; i++) {
                if (secret[i] == guess && display[i] == '_') {
                    display[i] = guess;
                    remaining--;
                }
            }
        } else {
            insert_sorted(eliminated, guess);
            wrong++;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("%s", ERR_MISSING_SECRET);
        return 1;
    }

    const char *secret = argv[1];

    // Enforce length limit
    size_t n = strlen(secret);
    if (n > MAX_SECRET_LENGTH) {
        printf("%s", ERR_SECRET_TOO_LONG);
        return 1;
    }

    // Secret must be lowercase letters + special chars only
    // (uppercase/digits in SECRET are invalid)
    if (!is_valid_secret(secret)) {
        printf("%s", ERR_INVALID_SECRET);
        return 1;
    }

    run_hangman(secret);
    return 0;
}