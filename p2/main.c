#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(char *format, ...);
void help();
int *argv_to_int(int argc, char **argv);
bool check_if_in_array(int *array, int array_length, int item);

/*
 * Usage:
 * ./main <file_path> <separator> <header_number[]>
 */
int main(int argc, char **argv) {
  if (argc < 4) {
    help();
    error("not enough arguments");
  }

  char *file_path = argv[1];
  char separator = argv[2][0];

  int header_indexes_len = argc - 3;
  int *header_indexes = argv_to_int(header_indexes_len, argv + 3);

  FILE *file = fopen(file_path, "r");
  if (!file) {
    error("%s could not be found", file_path);
  }

  char *line = NULL;
  size_t size = 0;
  while (getline(&line, &size, file) != -1) {
    int current_word_count = 0;
    int offset = 0;
    bool writed = false;
    for (int i = 0; i < strlen(line); i++) {
      if (line[i] == separator || line[i] == '\n' || line[i] == '\0') {
        char *word = malloc(i - offset + 1);
        memcpy(word, line + offset, i - offset);
        word[i - offset] = '\0';

        if (check_if_in_array(header_indexes, header_indexes_len,
                              current_word_count + 1)) {
          printf("%s\t", word);
          writed = true;
        }

        free(word);
        offset = i + 1;
        current_word_count++;
      }
    }
    if (writed)
      printf("\n");
  }

  fclose(file);
  return 0;
}

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

bool check_if_in_array(int *array, int array_length, int item) {
  for (int i = 0; i < array_length; i++) {
    if (array[i] == item)
      return true;
  }
  return false;
}
