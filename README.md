# dado

`dado`[^1] is a random passphrase generator based on the
[diceware](https://theworld.com/~reinhold/diceware.html) method.

## Installation:

Clone this repository and do:

```
make install
```

This will by default install the program to `/usr/local/bin`. You can change
that by adding `PREFIX=~/.local` (for example) to the previous command.

## Usage:

Invoking `dado` with the `-h` will display a usage text with details about the
available options.

### Options:

- `-l`: The passphrase's language. For now may be `fr` (French) or `en`
  (English). The later being the default when this option is not specified.
- `-n`: Number of words in the generated random passphrase. If not specified, it
  is by default set to 5.
- `-s`: A character used to separate the words of the generated passphrase. By
  default (when not specified) it is a single space.

## Credits:

The word lists used in this program are:

- The French list is collected by me. The code is in the
  [yafrdw](https://github.com/ewd340/yafrdw/) repository.
- The English list is the [EFF
  Large](https://www.eff.org/files/2016/07/18/eff_large_wordlist.txt) one

## Disclaimer:

This code in `src/dado.c` is written for my own use. Use it at your own risk. It
is provided as is, without warranty of any kind, and put in the public domain
(please see the UNLICENSE file). Your contributions, ideas, fixes, and
suggestions are most welcome.

[^1]: `dado` is the Italian word for `dice`.
