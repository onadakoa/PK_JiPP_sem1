#include <stdio.h>
#include <tui.h>

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
    scanf("%s", option);
    if (option[0] < '0' || option[0] > '9') {
      printf("wrong menu argument\n");
      print_menu();
      continue;
    }
    break;
  }

  return option[0] - '0';
}
