/// INCLUDES ///
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// DEFINES//
#define CTRL_KEY(k) ((k) & 0x1f)

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
  struct termios raw =
      orig_termios; // copy orig into a new variable to be modified

  raw.c_iflag &= (~BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_iflag &= ~(IXON | ICRNL);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

  raw.c_cc[VMIN] = 0;  // min byte count needed before read() can return
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
    if (c == CTRL_KEY('q'))
      break;
  }
  return 0;
}
