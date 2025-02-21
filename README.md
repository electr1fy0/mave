# Intro
Building my own text editor following [antirez's guide](https://viewsourcecode.org/snaptoken/kilo/).

# References
[unistd.h Man page](https://pubs.opengroup.org/onlinepubs/7908799/xsh/unistd.h.html)
[termios Man page](https://linux.die.net/man/3/termios)

# Progress
- [x] Setup
- [x] Entering raw mode
- [ ] Raw input and output
- [ ] A text viewer
- [ ] A text editor
- [ ] Search
- [ ] Syntax highlighting


# Notes
** will format them later **
int tcgetattr(int fd, struct termios *termios_p);

Retrieves the terminal configuration and stores it in a struct termios.

Parameters:
 • fd → File descriptor of the terminal (usually STDIN_FILENO, which is 0).
 • termios_p → Pointer to a struct termios that stores the settings.

Return Value:
 • 0 → Success
 • -1 → Error (sets errno)

errno

A global variable in C that stores an error code when a system call or library function fails.

Standard File Descriptors

Macro Value Description
STDIN_FILENO 0 Standard input
STDOUT_FILENO 1 Standard output
STDERR_FILENO 2 Standard error

perror

Prints an error message describing the last errno value.

perror("Error message");
