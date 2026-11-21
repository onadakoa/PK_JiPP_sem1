#include "tui.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <student.h>

char *StudentDataFieldName[3] = {"NAME", "SURNAME", "BIRTH_YEAR"};

StudentData *sd_fread_one(FILE *stream) {
  if (stream == NULL)
    return NULL;

  StudentData *out = calloc(1, sizeof(StudentData));
  if (out == NULL) {
    fprintf(stderr, "Error: can't calloc\n");
    exit(-1);
  }

  char *line = xmalloc(MAX_LINE_LENGTH);
  for (int i = 0; i < 3; i++) {

    if (fgets(line, MAX_LINE_LENGTH, stream) == NULL) {
      free(line);
      sd_free(&out);
      return NULL;
    }

    size_t line_len = strlen(line);
    if (line_len > 0 && line[line_len - 1] == '\n')
      line[--line_len] = '\0';
    if (line_len > 0 && line[line_len - 1] == '\r')
      line[--line_len] = '\0';

    switch ((StudentDataField)i) {
    case NAME:
      out->name = xmalloc(strlen(line) + 1);
      strcpy(out->name, line);
      break;
    case SURNAME:
      out->surname = xmalloc(strlen(line) + 1);
      strcpy(out->surname, line);
      break;
    case BIRTH_YEAR:
      if (strlen(line) != 4) {
        // printf("WARN [student]: birth_year should have only 4 chars...\n");
      }
      out->birth_year = xmalloc(strlen(line) + 1);
      strcpy(out->birth_year, line);
      break;
    }
  }

  free(line);
  return out;
}

char *sd_cpy_or_empty(char *a) {
  if (a == NULL)
    a = "";
  size_t len = strlen(a);
  char *out = xmalloc(len + 1);
  memcpy(out, a, len + 1);
  return out;
}
StudentData *sd_ask() {
  printf("NAME: ");
  char *name = line_input();
  if (name == NULL)
    return NULL;

  printf("SURNAME: ");
  char *surname = line_input();
  if (surname == NULL) {
    free(name);
    return NULL;
  }

  printf("BIRTH_YEAR: ");
  char *year = line_input();
  if (year == NULL) {
    free(name);
    free(surname);
    return NULL;
  }
  if (strlen(year) != 4) {
    printf("Note: birth years are usually 4 digits long\n");
  }

  StudentData *data = xmalloc(sizeof(StudentData));

  data->name = name;
  data->surname = surname;
  data->birth_year = year;

  return data;
}

void sd_print(StudentData *obj) {
  printf("%s\n%s\n%s\n", obj->name, obj->surname, obj->birth_year);
}

void sd_print_horizontal(StudentData *obj, const char *separator) {
  char *tmp[] = {obj->name, obj->surname, obj->birth_year};

  for (int i = 0; i < 3; i++) {
    printf("%s: %s", StudentDataFieldName[i], tmp[i]);
    if (i < 2)
      printf("%s", separator);
  }
}

void sd_free(StudentData **obj) {
  if (*obj == NULL)
    return;
  free((*obj)->name);
  free((*obj)->surname);
  free((*obj)->birth_year);
  free((*obj));
  *obj = NULL;
}
void sd_free_content(StudentData *obj) {
  if (obj == NULL)
    return;
  free(obj->name);
  free(obj->surname);
  free(obj->birth_year);
}

char *sd_read_fstring(FILE *stream) {
  if (stream == NULL)
    return NULL;
  size_t len = 0;

  if (fread(&len, sizeof(size_t), 1, stream) != 1 || len == 0)
    return NULL;

  char *str = xmalloc(len + 1);
  if (fread(str, 1, len, stream) != len) {
    free(str);
    return NULL;
  }
  str[len] = '\0';

  return str;
}
