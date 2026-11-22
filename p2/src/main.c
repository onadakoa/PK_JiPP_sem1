#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024

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
  if (strlen(argv[2]) != 1)
    error("wrong separator\n");
  char separator = argv[2][0];

  int header_indexes_len = argc - 3;
  int *header_indexes = argv_to_int(header_indexes_len, argv + 3);

  FILE *file = fopen(file_path, "r");
  if (!file) {
    error("%s could not be found", file_path);
  }

  printf("file path (for output): ");
  char *path = malloc(255);
  if (path == NULL)
    error("can't malloc");
  scanf("%253s", path);
  FILE *output = fopen(path, "w");
  free(path);
  if (output == NULL) {
    error("can't open file\n");
  }

  char *line = malloc(MAX_LINE_LEN);
  if (line == NULL)
    error("can't malloc");
  while (fgets(line, MAX_LINE_LEN, file) != NULL) {
    // while (getline(&line, &size, file) != -1) {
    int current_word_count = 0;
    int offset = 0;

    int writed = 0;
    for (int i = 0; i < strlen(line); i++) {
      if (line[i] == separator || line[i] == '\n' || line[i] == '\0') {
        char *word = malloc(i - offset + 1);
        if (word == NULL)
          error("can't malloc");

        memcpy(word, line + offset, i - offset);
        word[i - offset] = '\0';

        if (check_if_in_array(header_indexes, header_indexes_len,
                              current_word_count + 1)) {
          printf("%s\t", word);
          fprintf(output, "%s\t", word);
          writed = 1;
        }

        free(word);
        offset = i + 1;
        current_word_count++;
      }
    }
    if (writed) {
      printf("\n");
      fprintf(output, "\n");
    }
  }

  free(line);
  free(header_indexes);
  fclose(file);
  fclose(output);
  return 0;
}
