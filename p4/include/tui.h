#ifndef TUI_H
#define TUI_H

#include <stddef.h>
enum MY_INTERF_EN {
  INTERF_PUSH = 0,
  INTERF_POP,
  INTERF_CLEAR,
  INTERF_FIND,
  INTERF_SAVE_TO_FILE,
  INTERF_READ_FROM_FILE,
  INTERF_DISPLAY,
  INTERF_COUNT,
  INTERF_STOP,
  // INTERF_TOT
};
extern char strtab[9][20];

extern void print_menu();
extern int ask_menu(const char *prompt);
extern char *line_input();

void *xmalloc(size_t size);

#endif // !TUI_H
