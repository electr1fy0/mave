/// INCLUDES ///

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>



// NOTES //
// int tcgetattr(int fd, struct termios *termios_p);
// This retrieves the terminal config and stores it in a struct of
// type termios.
// fd stands for file descriptor of the terminal (usually STDIN_FILENO
// which is 0)
// termios_p is a pointer to a struct termios
// return value:
// 0 is success -1 sets errno
//
// errno is a global var in C that stores error code when a system call or library fn
// fails
// STDIN_FILENO (macro)
// 0 for stdin
// 1 for stdout
// 2 for std error
//
// perror is description of errno, print error funny syntax tbh


/// DATA ///
struct termios orig_termios;

/// TERMINAL ///
void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
      // “Terminal Control Set Attributes - Flush” // set after flush
    die("tcsetattr");
}

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    die("tcgetattr");
  atexit(disableRawMode);
  struct termios raw = orig_termios; // copy orig into a new variable to be modified


  //
  raw.c_iflag &= (~BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_iflag &= ~(IXON | ICRNL);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

  raw.c_cc[VMIN] = 0; // min byte count needed before read() can return
  raw.c_cc[VTIME] = 1; // max time to wait before read() returns
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tcsetattr");
}


/// INIT ///
int main() {
  enableRawMode();
  char c;
  while (1) {
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
      die("read");
    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }
    if (c == 'q')
      break;
  }
  return 0;
}
