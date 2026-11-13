#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <student.h>

char *StudentDataFieldName[3] = {"NAME", "SURNAME", "BIRTH_YEAR"};

StudentData *sd_fread_one(FILE *stream) {
  if (stream == NULL)
    return NULL;

  StudentData *out = malloc(sizeof(StudentData));

  char *line = malloc(MAX_LINE_LENGTH);
  for (int i = 0; i < 3; i++) {

    if (fgets(line, MAX_LINE_LENGTH, stream) == NULL) {
      free(line);
      free(out);
      return NULL;
    }

    size_t line_len = strlen(line);
    if (line[line_len - 2] == '\r')
      line[line_len - 2] = '\0';
    else if (line[line_len - 1] == '\n')
      line[line_len - 1] = '\0';

    switch ((StudentDataField)i) {
    case NAME:
      out->name = malloc(strlen(line) + 1);
      strcpy(out->name, line);
      break;
    case SURNAME:
      out->surname = malloc(strlen(line) + 1);
      strcpy(out->surname, line);
      break;
    case BIRTH_YEAR:
      if (strlen(line) != 4) {
        // printf("WARN [student]: birth_year should have only 4 chars...\n");
      }
      out->birth_year = malloc(strlen(line) + 1);
      strcpy(out->birth_year, line);
      break;
    }
  }

  free(line);
  return out;
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
  free((*obj)->name);
  free((*obj)->surname);
  free((*obj)->birth_year);
  free((*obj));
  obj = NULL;
}
