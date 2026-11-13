#ifndef STUDENTS_H
#define STUDENTS_H

#define MAX_LINE_LENGTH 255

#include <stdio.h>

typedef enum {
  NAME = 0,
  SURNAME,
  BIRTH_YEAR,
} StudentDataField;
extern char *StudentDataFieldName[3];

typedef struct {
  char *name;
  char *surname;
  char *birth_year;
} StudentData;

StudentData *sd_fread_one(FILE *stream);
StudentData *sd_ask();

void sd_print(StudentData *obj);
void sd_print_horizontal(StudentData *obj, const char *separator);

void sd_free(StudentData **obj);
#endif // !STUDENTS_H
