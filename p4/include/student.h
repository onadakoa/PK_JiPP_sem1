#ifndef STUDENTS_H
#define STUDENTS_H

#define MAX_LINE_LENGTH 255

#include <stdio.h>

typedef enum {
  NAME = 0,
  SURNAME,
  BIRTH_YEAR,
} StudentDataField;

typedef struct {
  char *name;
  char *surname;
  char *birth_year;
} StudentData;

StudentData *fsd_read_one(FILE *stream);

void sd_print(StudentData *obj);
void sd_print_horizontal(StudentData *obj, const char separator);

void sd_free(StudentData **obj);
#endif // !STUDENTS_H
