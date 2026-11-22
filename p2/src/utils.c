#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void error(char *format, ...) {
  va_list list;
  va_start(list, format);

  printf("\nError:\n\t");
  vprintf(format, list);

  va_end(list);
  exit(1);
}

void help() {
  printf("Usage:\n\t");
  printf("./main <file_path> <separator> <header_number[]>");
}

int *argv_to_int(int argc, char **argv) {
  int *out = malloc(sizeof(int) * argc);
  for (int i = 0; i < argc; i++) {
    if ((out[i] = atoi(argv[i])) <= 0) {
      error("one of arguments is out of bounds, must be greater than 0");
    }
  }

  return out;
}

int check_if_in_array(int *array, int array_length, int item) {
  for (int i = 0; i < array_length; i++) {
    if (array[i] == item)
      return 1;
  }
  return 0;
}
