#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tui.h>

#define TUI_MAX_LINE_INPUT 1024

char strtab[9][20] = {
    "0 - push",           // INTERF_PUSH
    "1 - pop",            // INTERF_POP
    "2 - clear",          // INTERF_CLEAR
    "3 - find field",     // INTERF_find
    "4 - save to file",   // INTERF_SAVE_TO_FILE
    "5 - read from file", // INTERF_READ_FROM_FILE,
    "6 - print",          // INTERF_DISPLAY
    "7 - count",          // INTERF_COUNT
    "8 - finish"          // INTERF_STOP
};

void print_menu() {
  printf("\nmenu:\n");
  for (int i = 0; i < 9; i++) {
    printf("\t%s\n", strtab[i]);
  }
}

int ask_menu(const char *prompt) {
  char option[4] = "";
  while (1) {
    printf("\n%s", prompt);
    if (fgets(option, sizeof(option), stdin) == NULL) {
      return -1;
    }
    if (option[0] < '0' || option[0] > '9') {
      printf("wrong menu argument\n");
      print_menu();
      continue;
    }
    break;
  }

  return option[0] - '0';
}

void clear_stdin() {
  int c;
  while ((c = getc(stdin)) != '\n' && c != EOF) {
  }
}

char *line_input() {
  char line[TUI_MAX_LINE_INPUT];
  if (fgets(line, TUI_MAX_LINE_INPUT, stdin) == NULL) {
    printf("error handling input\n");
    return NULL;
  }
  size_t len = strlen(line);
  if (len < 1)
    return NULL;
  if (line[len - 1] == '\n') {
    line[len - 1] = '\0';
  } else {
    clear_stdin();
  }

  len = strlen(line);
  char *out_line = xmalloc(len + 1);
  memcpy(out_line, line, len + 1);

  return out_line;
}

void *xmalloc(size_t size) {
  void *p = malloc(size);
  if (p == NULL) {
    fprintf(stderr, "\n\nError: can't malloc\n\n'");
    exit(-1);
  }
  return p;
}
