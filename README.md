*This project has been created as part of the 42's curriculum by fhidalgo*

# Get Next Line

## Table of contents

- [Description](#description)
- [Instructions](#instructions)
- [Resources](#resources)
- [Theory](#theory)
  - [What is a file descriptor?](#what-is-a-file-descriptor)
    - [Standard file descriptors](#standard-file-descriptors)
  - [Key functions](#key-functions)
    - [read](#read)
    - [open](#open)
  - [What is a static variable?](#what-is-a-static-variable)
  - [What is the difference between a buffer and `malloc`?](#what-is-the-difference-between-a-buffer-and-malloc)
- [Explaining functions](#explaining-functions)
  - [The utils](#the-utils)
  - [The main functions](#the-main-functions)
- [The journey of the text](#the-journey-of-the-text)
- [The bonus part](#the-bonus-part)

## Description

Welcome to my Get Next Line project. It is a function that returns **one line per call** from a file descriptor, no matter how big the file is or what `BUFFER_SIZE` we choose.

```c
char *get_next_line(int fd);
```

- It returns the line read, **including the `'\n'`** (if there is one).
- It returns `NULL` when there is nothing else to read, or if an error happens.
- The last line of a file may not end in `'\n'`.

The project is made of three files:

| File | Content |
|------|---------|
| `get_next_line.c` | `get_next_line` and its three helpers: `get_stash`, `get_line` and `get_rest` |
| `get_next_line_utils.c` | `ft_strlen`, `char_index_finder`, `ft_substr`, `ft_strdup`, `ft_strjoin_and_freeold` |
| `get_next_line.h` | Prototypes and the default `BUFFER_SIZE` |

The key idea is a `static` variable (`stash`) that keeps what we have read but not returned yet, so it survives between calls.

## Instructions

### Compiling

The project has no Makefile. We compile it together with our own `main`, choosing the buffer size with `-D`:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

If `-D BUFFER_SIZE` is not given, the header defines it as `3`.

### Running

```bash
./gnl
```

Example of a `main` for testing:

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "get_next_line.h"

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;

	if (argc != 2)
		return (1);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

Try different sizes (`-D BUFFER_SIZE=1`, `=3`, `=10000`) and check leaks with `valgrind --leak-check=full ./gnl file.txt`.

## Resources

### References

- [`read(2)`](https://man7.org/linux/man-pages/man2/read.2.html): return value (`> 0` bytes read, `0` end of file, `-1` error).
- [`open(2)`](https://man7.org/linux/man-pages/man2/open.2.html) and [`close(2)`](https://man7.org/linux/man-pages/man2/close.2.html)
- [`malloc(3)` / `free(3)`](https://man7.org/linux/man-pages/man3/malloc.3.html)
- [Valgrind Manual](https://valgrind.org/docs/manual/manual.html): leak checking and invalid reads/writes.
- [cppreference: storage duration](https://en.cppreference.com/w/c/language/storage_duration): `static` variables, the base of this project.
- [Beej's Guide to C Programming](https://beej.us/guide/bgc/): pointers, strings and dynamic memory.
- Kernighan & Ritchie, *The C Programming Language*.

### Use of AI
AI was used for:
- **Debugging**: finding specific errors in some programs, mainly memory allocation issues (`malloc`/`free`, leaks).
- **Theory**: explaining specific details about pointers and static variables, as a more precise alternative to searching online.
- **README.md**: formatting and structuring the sections.

No AI-generated code was submitted without being understood and reviewed.

## Theory

### What is a file descriptor?

When a program runs, it uses RAM for many things, including references to files. Those references live in the **kernel space**.

> The **kernel space** is an area of memory reserved EXCLUSIVELY for the kernel.

If a program uses two files, each one gets its own entry in the kernel space. To tell them apart from our code, we use the `fd` (file descriptor).

A `fd` is just an `int`. It is a normal variable, so it lives in the **stack**.

> The **stack** is the area of memory reserved for a running program. The program's **functions and local variables** live there.

The kernel always gives the **lowest number not in use**. The first three are already taken by the standard descriptors, so the first file we open gets `fd = 3`, the next one `fd = 4`, and so on. If we `close` one, that number becomes free and can be reused.

When the program ends, its memory is released and the kernel closes the descriptors it still had open.

#### Standard file descriptors

| Name | `fd` | Meaning |
|------|------|---------|
| `STDIN` | 0 | Standard input. By default, the keyboard. |
| `STDOUT` | 1 | Standard output. What the terminal shows, except errors. |
| `STDERR` | 2 | Standard error. The error messages. |

> The computer treats what we type **as if it were a file**, that is why the keyboard has its own `fd`.

`STDOUT` and `STDERR` are separated so we can handle normal output and errors independently (for example, redirecting one to a file and keeping the other on screen).

### Key functions

#### read

```c
ssize_t read(int fd, void *buf, size_t count);
```

It receives:
- The `fd` of the file.
- A pointer to a **memory area** that we have already reserved (`buf`).
- The **maximum number of bytes** we ask it to read (`count`).

It returns a `ssize_t` ("signed size_t"):
- `> 0`: the exact number of bytes read and copied into the buffer. It can be **less** than `count`.
- `= 0`: end of file, there is nothing else to read.
- `= -1`: an error happened.

Also, `read` **does not add a `'\0'`** to what it reads. That is why in `get_stash` we reserve `BUFFER_SIZE + 1` and add it ourselves.

#### open

It gives a `fd` to a file so our program can use it. It is a **variadic** function: it can receive a variable number of arguments (two or three).

```c
int open(const char *pathname, int flags, ...);
```

It normally receives:
- A string with the path of the file.
- Flags that say **how** to open it.

Optionally, when the file is created, it also receives the file **permissions**, in octal (like in `chmod`).

```c
int fd = open("new.txt", O_CREAT | O_WRONLY, 0644);
```

> The leading `0` in `0644` is what tells C that the number is octal. Without it, `644` would be read as decimal and the permissions would be wrong.

**Access modes** (exactly one of them is required):
- `O_RDONLY`: read only.
- `O_WRONLY`: write only.
- `O_RDWR`: read and write.

`O_CREAT` is not an access mode but an extra flag: it creates the file if it doesn't exist. It is combined with the others using `|`. For this project we only need `O_RDONLY`.

### What is a static variable?

A variable whose value **doesn't die when the function ends**: it is kept between calls and lives during the whole program. Its name is only visible inside the function (or file) where it is declared. In `get_next_line`, `stash` is static, so it remembers the text we read but haven't returned yet.

### What is the difference between a buffer and `malloc`?

They are not opposites. A **buffer** is just a memory area used to temporarily store data (here, what `read` gives us). `malloc` is one **way of getting** that memory.

- A buffer can be a fixed-size array in the **stack** (`char buf[BUFFER_SIZE + 1];`). It is freed automatically when the function ends.
- Or it can be reserved with `malloc` in the **heap**. It stays alive until we call `free`, so we must free it ourselves. That is what `get_stash` does.

The heap also belongs to our program, but its size is decided while the program is running.

## Explaining functions

### The utils

#### ft_strlen

It measures the length of a string, without counting the `'\0'`.

#### char_index_finder

It finds the position of the first appearance of the character `c` in the string `s`, starting from the index `i`. It works like `ft_strchr`, but returns an **index** instead of a pointer.

- We first check that `s` exists, that `i` is not negative and that `i` is not beyond the end of the string. If something fails, we return `-1`.

- We loop while `s[i]` is not the `'\0'` and is different from `c`, moving `i` forward. We cast `c` to `char` because it comes as an `int`.

- When the loop ends, we check why. If `s[i] == (char)c` we found it, so we return `i`. If not, we reached the end of the string without finding it, so we return `-1`. We use that value for error handling.

```
s = "hello\nworld"   c = '\n'   i = 0

index:  0 1 2 3 4 5  6 ...
        h e l l o \n w ...
                  ^
                  returns 5
```

#### ft_substr

It reserves memory and returns a substring of `s`, starting at `start` and with a maximum length of `len`.

- If `s` doesn't exist, we return `NULL`.

- If `start` is beyond the end of `s`, there is nothing to copy, so we return an empty string (`ft_strdup("")`). That is not an error. We will use it later in `get_rest`.

- If `len` is bigger than what is left of `s`, we shorten it.

- We reserve `len + 1` bytes, copy the characters and add the `'\0'`.

#### ft_strdup

It returns a new string that is a copy of `src` (reserving `length + 1` bytes for the `'\0'`). Here it is only used to create the empty string `""` in `ft_substr`.

#### ft_strjoin_and_freeold

It joins `s1` and `s2` into a new string **and frees `s1`**. That is exactly what we need: every time we read, the old `stash` becomes useless, so we free it while creating the bigger one.

- If `s1` is `NULL` (the very first read, when the static variable is still empty), we reserve only the size of `s2` plus the `'\0'`. If not, we reserve both lengths plus one.

- If `malloc` fails, we free `s1` and return `NULL`, so we don't leak the old stash.

- We copy `s1` (if it exists) and then `s2`, add the `'\0'` and free `s1`.

### The main functions

#### get_stash

It reads from `fd` and keeps adding to the `stash` **until there is a `'\n'` in it or the file ends**.

- We reserve a buffer `buf` of `BUFFER_SIZE + 1` bytes (the `+1` is for the `'\0'`, since `read` doesn't add it).

- We loop while `reader > 0` and there is no `'\n'` in the stash (`char_index_finder(stash, '\n', 0) == -1`).

- Inside the loop we call `read`. If it returns a negative number, it is an error: we free `buf` and `stash` and return `NULL`.

- We add the `'\0'` at `buf[reader]`, so `buf` is a valid string, and join it to the stash.

- When the loop ends we free `buf` and return the stash. It may contain **more than one line**: `read` doesn't know where lines end.

```
BUFFER_SIZE = 4

stash: NULL
read -> "hell"         stash = "hell"       (no '\n', keep reading)
read -> "o\nwo"        stash = "hello\nwo"  ('\n' found, stop)
```

#### get_line

It extracts from the stash the first line, that is, everything up to the first `'\n'` (included).

- If the stash doesn't exist or is empty, there is nothing to return, so we return `NULL`. That is how we tell the caller that the file ended.

- We look for the `'\n'` with `char_index_finder`. If it is at position `p`, the line has `p + 1` characters (the `'\n'` is included).

- If there is no `'\n'`, we are in the last line of the file, so the line is the whole stash (`ft_strlen`).

- We return `ft_substr(stash, 0, len)`: a **new** string. The stash is not modified yet.

```
stash = "hello\nwo"
         01234 5
'\n' at 5  ->  len = 6  ->  line = "hello\n"
```

#### get_rest

It builds the new stash: what was left after the first line.

- If there is no `'\n'`, we returned the entire stash as a line, so nothing is left. We free it and return `NULL`.

- Otherwise, we copy from `p + 1` till the end with `ft_substr`, and we free the old stash.

- If what is left is empty (the line ended exactly at the end of the stash), we free it and return `NULL`. This way the next call starts clean.

```
stash = "hello\nwo"    '\n' at 5

rest  = "wo"           (from index 6 till the end)
```

#### get_next_line

It puts everything together.

- If `fd` is negative or `BUFFER_SIZE` is not positive, we return `NULL`.

- `stash` is `static`, so it **keeps its value between calls**. That is what lets us remember the text we read but didn't return.

- Steps: `get_stash` fills it, `get_line` extracts the line, `get_rest` updates the stash.

- If there is no line, we free the stash, set it to `NULL` and return `NULL`, so the next call with another file starts from zero.

## The journey of the text

Let's follow a file with this content, using `BUFFER_SIZE = 4`:

```
hello\nworld\n
```

**Call 1**

```
static stash: NULL

1. get_stash   read "hell"  -> stash = "hell"        no '\n', read again
               read "o\nwo" -> stash = "hello\nwo"    '\n' found, stop
2. get_line    stash -> "hello\n"                     returned to the user
3. get_rest    stash -> "wo"                          saved in the static variable
```

**Call 2**

```
static stash: "wo"

1. get_stash   "wo" has no '\n', so we read
               read "rld\n" -> stash = "world\n"      '\n' found, stop
2. get_line    stash -> "world\n"                     returned
3. get_rest    nothing after the '\n' -> NULL         the static variable is empty
```

**Call 3**

```
static stash: NULL

1. get_stash   read returns 0 (end of file) -> stash = ""
2. get_line    empty stash -> NULL
3. get_next_line frees the stash and returns NULL     the user knows it is over
```

Summary of the flow:

```
          fd
           |
        read() ──> buf ──┐
                         v
 static stash ──> ft_strjoin_and_freeold ──> stash (bigger)
                         |
              has a '\n'? ── no ──> read again
                         | yes (or EOF)
                         v
                     get_line ──> line (returned)
                         |
                         v
                     get_rest ──> new stash (kept for the next call)
```

The algorithm in one sentence: **read in blocks, accumulate until a `'\n'` appears, cut the first line out and save the leftovers for the next call.**

## The bonus part

All the thing was about accepting different files descriptors at the same time, right? So, the whole change in this version is that the `stash` is now a array of pointers. Why? For making an array of arrays, each index meaning one posible `fd` so we can always differenciate the file we are in.

For that we include another system library (`sys/select.h`) in our `.h` file for using the standaard max `fd` number (given that it is defined by the library). This library has the data `FD_SETSIZE` who will always provide a safe default max size for the array.

Once defined the lenght of our double pointer, we use each string in the double pointer as we did before. Of course, we had to change the prototypes of some functions for making them recieving a doble pointer when we pass the address of our `fd` string.

Here, a testing main:

```c

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "get_next_line_bonus.h"

int	main(int argc, char **argv)
{
	int		fd1;
	int		fd2;
	int		fd3;
	char	*line1;
	char	*line2;
	char	*line3;

	if (argc != 4)
		return (1);
	fd1 = open(argv[1], O_RDONLY);
	fd2 = open(argv[2], O_RDONLY);
	fd3 = open(argv[3], O_RDONLY);
	if (fd1 < 0 || fd2 < 0 || fd3 < 0)
		return (1);
	while (1)
	{
		line1 = get_next_line(fd1);
		line2 = get_next_line(fd2);
		line3 = get_next_line(fd3);
		if (!line1 && !line2 && !line3)
			break ;
		if (line1)
		{
			printf("[FD 1]: %s", line1);
			free(line1);
		}
		if (line2)
		{
			printf("[FD 2]: %s", line2);
			free(line2);
		}
		if (line3)
		{
			printf("[FD 3]: %s", line3);
			free(line3);
		}
	}
	close(fd1);
	close(fd2);
	close(fd3);
	return (0);
}

```