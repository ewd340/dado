// dado m (plural dadi): italian word for 'dice'.
// This program is a passphrase generator based on
// the diceware technique documented here:
// https://theworld.com/~reinhold/diceware.html

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "lists/en.h"
#include "lists/fr.h"

#define PROG "dado"
#define VERSION "1.0"
#define MAX_PHRASE 255 // Maximum size for a passphrase.
#define MIN_WORDS 1 // Maximum size for a passphrase.
#define MAX_WORDS 30 // Maximum size for a passphrase.
#define DEFAULT_LEN 5 // Default number of words in the passphrase.
#define DEFAULT_SEPARATOR ' '

#define XSTR(x) STR(x)
#define STR(x) #x
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
// Shortcut strings comparisons
#define STREQ(x, y) (!memcmp((x), (y), strlen((x))))
// Assign err_value to err (for return) and bail out to the 'bail' label.
#define BAIL(err_value) \
do { \
    err = (err_value); \
    goto bail; \
} while(0)

enum error {
    ERR_OK,
    ERR_NO_RANDOM,
    ERR_INVALID_ARG,
    ERR_WRONG_NUMBER,
};

static const char *errmsg[] = {
    [ERR_OK] = "All is well.",
    [ERR_NO_RANDOM] = "Could not gather enough entroy",
    [ERR_INVALID_ARG] = "Invalid argument, should be a number.",
    [ERR_WRONG_NUMBER] = "The number of words should be between "
                         XSTR(MIN_WORDS)" and " XSTR(MAX_WORDS),
};

static const char* usage = "USAGE:\n"
    "\tdado [-l language] [-n words] [-s sep]\n"
    "\n"
    "OPTIONS:\n"
    "\t-l\tLanguage of the generated passphrase (default 'en')\n"
    "\t-n\tNumber of words in the passphrase (default "XSTR(DEFAULT_LEN)")\n"
    "\t-s\tA character used to separate the words (default space)\n"
    "\n"
    "EXAMPLES:\n"
    "\tdado -n 5 -l fr\n"
    "\tdado -s'-' -n 6 -l en\n"
    "\tdado -n 4";


// Fill a buffer 'buf' with 'len' random bytes. Thanks @skeeto.
#ifdef _WIN32
static inline int fillrand(void *buf, int len)
{
    return !RtlGenRandom(buf, len);
}
#else // !_WIN32 
static inline int fillrand(void *buf, int len)
{
    FILE *f = fopen("/dev/random", "rb");
    if (!f) return 1;
    int r = !fread(buf, len, 1, f);
    fclose(f);
    return r;
}
#endif


// Generate a passphrase of 'len' words extracted from the list of words
// given in 'list' (which size is 'list_size') and separated with a character
// given in 'sep'. The generated passphrase is put in 'passphrase'.
enum error generate_passphrase(char *passphrase, const size_t len,
                               const char sep, const char **list,
                               const size_t list_size)
{
    enum error err = ERR_OK;
    uint16_t numbers[30];
    char *p = passphrase;

    if (fillrand(numbers, 2*len)) {
       BAIL(ERR_NO_RANDOM);
    }

    for (size_t i = 0; i < len; ++i) {
        uint16_t widx = numbers[i] % list_size;
        const char *word = list[widx];
        size_t wlen = strlen(word);
        memcpy(p, word, wlen) ;
        p += wlen;
        *p++ = sep;
    }
    *--p = '\0';
bail:
    return err;
}


int main(int argc, char *argv[])
{
    enum error err = ERR_OK;
    char sep = DEFAULT_SEPARATOR;
    char const **list = en;
    size_t list_size = ARRAY_SIZE(en);
    char *l = NULL;
    char passphrase[MAX_PHRASE];
    int len = DEFAULT_LEN;
    errno = 0;
    int option;

    while((option = getopt(argc, argv, "s:n:l:Vh")) != -1) {
        switch (option) {
            case 's':
                sep = optarg[0];
                break;
            case 'n': {
                char *p;
                len = strtol(optarg, &p, 10);
                if (errno || *p) {
                    BAIL(ERR_INVALID_ARG);
                }
                if (len < MIN_WORDS || len > MAX_WORDS) {
                    BAIL(ERR_WRONG_NUMBER);
                }
            }
                break;
            case 'l':
                l = optarg;
                if (STREQ(l, "fr") || STREQ(l, "FR")) {
                    list = fr;
                    list_size = ARRAY_SIZE(fr);
                } else {
                    list = en;
                    list_size = ARRAY_SIZE(en);
                }
                break;
            case 'V':
                fprintf(stdout, "%s %s\n", PROG, VERSION);
                BAIL(ERR_OK);
                break;
            case 'h':
                fprintf(stdout, "%s\n", usage);
                BAIL(ERR_OK);
                break;
            default:
                goto bail;
        }
    }

    err = generate_passphrase(passphrase, len, sep, list, list_size);

    if (err != ERR_OK) {
        BAIL(err);
    }
    printf("%s\n", passphrase);

bail:
    if (err != ERR_OK) {
        fprintf(stderr, "%s: %s\n", PROG, errmsg[err]);
        return 1;
    }
    return 0;
}
