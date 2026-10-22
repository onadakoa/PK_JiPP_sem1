#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help();
void error(const char *format, ...);
int *parse_argv_to_int(int argc, char **argv);
bool check_if_in_array(int item, int *array, int length);

/*
 * Usage:
 * ./main <file_path> <separator> <header_number[]>
 */
int main(int argc, char **argv) {
  if (argc <= 3) {
    printf("Error: less than 3 arguments specified\n");
    print_help();
    return 1;
  }
  char *path = argv[1];
  char *separator = argv[2];
  if (strlen(separator) != 1) {
    printf("Error: separator must be 1 character\n");
    exit(1);
  }

  int headers_len = argc - 3;
  int *headers = parse_argv_to_int(headers_len, argv + 3);

  //printf("path: %s\nsep: %s\n", path, separator);

  FILE *file;
  // TODO: it won't trigger for directories'
  if (!(file = fopen(path, "r"))) {
    printf("Error: couldn't open or find file %s", path);
    return 1;
  }

  char *line = NULL;
  size_t size = 0;
  while (getline(&line, &size, file) != -1) {
    unsigned int line_len = strlen(line);
    unsigned int word_number = 1;
    char *word;
    unsigned int offset = 0;
    bool printed = false;
    for (unsigned int i = 0; i <= line_len; i++) {
      if (line[i] == *separator || line[i] == '\n' || line[i] == '\0') {
        unsigned int len = i - offset;
        word = malloc(len + 1);
        memcpy(word, line + offset, len);
        word[len] = '\0';

        offset = i + 1;

        if (check_if_in_array(word_number, headers, headers_len)) {
          printf("%s\t", word);
          printed = true;
        }
        free(word);
        word_number++;
      }
    }
    free(line);
    if (printed)
      printf("\n");
  }

  fclose(file);
  return 0;
}

void print_help() {
  printf("Usage:\n");
  printf("./main <file_path> <separator> <header_number[]>\n");
}

void error(const char *format, ...) {
  va_list list;
  va_start(list, format);

  printf("\n\nError: ");
  vprintf(format, list);

  va_end(list);
  exit(1);
}

int *parse_argv_to_int(int argc, char **argv) {
  int *out = malloc(sizeof(int) * argc);

  for (int i = 0; i < argc; i++) {
    char *word = argv[i];
    //    printf("%s - %i %i", word, argc, i);
    char *endptr;

    long tmp = strtol(word, &endptr, 10);

    if (endptr == word || (*endptr != '\0' && *endptr != '\n')) {
      error("one of the arguments aren't integer'");
      exit(1);
    }
    if (tmp > INT_MAX || tmp < 1) {
      error("argument out of bounds");
      exit(1);
    }
    out[i] = (int)tmp;
  }

  return out;
}

bool check_if_in_array(int item, int *array, int length) {
  for (int i = 0; i < length; i++) {
    if (item == array[i])
      return true;
  }
  return false;
}
