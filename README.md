# libu8t

## Table of Contents
- [Description](#description)
- [Install](#install)
- [Usage](#usage)
- [Documentation](#documentation)
- [Maintainers](#maintainers)
- [Contributing](#contributing)

## Description
Tokenizer C library with UTF-8 support.

## Install
```bash
$ git clone https://git.sr.ht/~klahr/libu8t
$ cd libu8t
$ make
$ sudo make install
```
#### Uninstall
```bash
$ sudo make uninstall
```

## Usage
```c
// example.c

#include <u8t/scanner.h>

int main(void) {
	const char* src = "var x = 42;";

	u8t_scanner s;
	u8t_scanner_init(&s, src);

	char32_t token;
	while ((token = u8t_scanner_scan(&s)) != U8T_EOF) {
		size_t n;
		switch (token) {
			case U8T_IDENTIFIER:
				printf("Identifier: %s\n", u8t_scanner_token_text(&s, &n));
				break;
			case U8T_INTEGER:
				printf("Integer   : %s\n", u8t_scanner_token_text(&s, &n));
				break;
			case U8T_STRING:
				printf("String    : %s\n", u8t_scanner_token_text(&s, &n));
				break;
			case U8T_FLOAT:
				printf("Float     : %s\n", u8t_scanner_token_text(&s, &n));
				break;
			default:
				printf("Token     : %c\n", token);
				break;
		}
	}

	return 0;
}
```
```bash
$ gcc -o example -lu8t example.c
```
```bash
$ ./example
Identifier: var
Identifier: x
Token     : =
Integer   : 42
Token     : ;
```

## Documentation
The API documentation can be generated using Doxygen. Files are located in the `docs/` directory.
```back
$ doxygen Doxyfile
```

## Maintainers
[~klahr](https://sr.ht/~klahr)

## Contributing
We enthusiastically encourage and welcome contributions.

[nova-r8]: https://git.sr.ht/~klahr/nova-r8

