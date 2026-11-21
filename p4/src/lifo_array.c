#include "student.h"
#include "tui.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StudentData *sd_array = NULL;
size_t sd_array_len = 0;

void merge_left();
void merge_right();

int add_item(StudentData data);
int pop_item(StudentData *data);

int count_items();
int search_item(StudentData *data, StudentDataField field, char *value);

void print_items();
void free_items();

int write_items(FILE *stream);
int read_items(FILE *stream);

void h_add();
void h_pop();

void h_find();

void h_write();
void h_read();

int main(int argc, char **argv) {
  if (argc > 1) {
    char *path = argv[1];
    FILE *file = fopen(path, "r");
    if (file == NULL) {
      printf("couldn't open '%s'", path);
      return 1;
    }

    StudentData *student = NULL;
    while ((student = sd_fread_one(file)) != NULL) {
      add_item(*student);
      free(student);
    }
    fclose(file);
    printf("found %d items in %s\n\n", count_items(), path);
  }

  print_menu();
  int option = 0;
  while ((option = ask_menu("[LIFO array] ")) >= 0) {
    switch (option) {
    case INTERF_PUSH:
      h_add();
      break;
    case INTERF_POP:
      h_pop();
      break;
    case INTERF_CLEAR:
      free_items();
      printf("new count: %zu\n", sd_array_len);
      break;
    case INTERF_FIND:
      h_find();
      break;
    case INTERF_SAVE_TO_FILE:
      h_write();
      break;
    case INTERF_READ_FROM_FILE:
      h_read();
      break;
    case INTERF_DISPLAY:
      print_items();
      break;
    case INTERF_COUNT:
      printf("count: %i\n", count_items());
      break;
    case INTERF_STOP:
      free_items();
      break;

    default:
      printf("wrong menu option\n");
      print_menu();
    }

    if (option == INTERF_STOP)
      break;
  }
}

int add_item(StudentData data) {
  if (sd_array == NULL || sd_array_len == 0) {
    sd_array = malloc((sizeof(StudentData)));
    sd_array_len = 1;
    sd_array[0] = data;
    return 0;
  }
  sd_array_len++;
  sd_array = realloc(sd_array, sizeof(StudentData) * sd_array_len);

  merge_right();
  sd_array[0] = data;
  return 0;
}

int pop_item(StudentData *data) {
  if (sd_array == NULL || sd_array_len < 1)
    return -1;
  *data = sd_array[0];
  merge_left();

  sd_array_len--;
  sd_array = realloc(sd_array, sizeof(StudentData) * sd_array_len);

  return 0;
}

int count_items() { return (int)sd_array_len; }

void print_items() {
  for (size_t i = 0; i < sd_array_len; i++) {
    printf("%zu: ", i);
    sd_print_horizontal(&sd_array[i], ", ");
    printf("\n");
  }
}

int search_item(StudentData *data, StudentDataField field, char *value) {
  if (sd_array == NULL || sd_array_len < 1)
    return -1;

  for (int i = 0; i < count_items(); i++) {
    StudentData tmp = sd_array[i];
    char *fields[3] = {tmp.name, tmp.surname, tmp.birth_year};

    if (strcmp(fields[field], value) == 0) {
      *data = tmp;
      return 1;
    }
  }

  return 0;
}

void free_items() {
  if (sd_array == NULL || sd_array_len == 0)
    return;

  for (size_t i = 0; i < sd_array_len; i++) {
    sd_free_content(&sd_array[i]);
  }

  free(sd_array);
  sd_array = NULL;
  sd_array_len = 0;
}

void merge_right() {
  if (sd_array == NULL || sd_array_len < 2)
    return;

  for (size_t i = 0; i < sd_array_len - 1; i++) {
    size_t cursor = sd_array_len - 1 - i;
    sd_array[cursor] = sd_array[cursor - 1];
  }
}

void merge_left() {
  if (sd_array == NULL || sd_array_len <= 1)
    return;

  for (int i = 0; i < count_items() - 1; i++) {
    sd_array[i] = sd_array[i + 1];
  }
}

int write_items(FILE *stream) {
  if (stream == NULL || sd_array == NULL || sd_array_len < 1)
    return -1;

  for (int i = 0; i < count_items(); i++) {
    char *fields[3] = {sd_array[i].name, sd_array[i].surname,
                       sd_array[i].birth_year};

    for (int i = 0; i < 3; i++) {
      int len = strlen(fields[i]);
      fwrite(&len, sizeof(int), 1, stream);
      fwrite(fields[i], 1, len, stream);
    }
  }

  return 0;
}

int read_items(FILE *stream) {
  if (stream == NULL || sd_array != NULL || sd_array_len != 0)
    return -1;

  while (1) {
    StudentData it = {0};
    char **fields[3] = {&it.name, &it.surname, &it.birth_year};
    for (int i = 0; i < 3; i++) {
      char *str = sd_read_fstring(stream);
      if (str == NULL) {
        sd_free_content(&it);

        if (feof(stream)) {
          printf("finished file\n");
          return 0;
        } else {
          printf("error handling file\n");
          return 1;
        }
      }
      *fields[i] = str;
    }

    add_item(it);
  }

  return 0;
}

void h_add() {
  printf("item prompt\n");
  StudentData *it = sd_ask();
  add_item(*it);
  free(it);
  printf("item added\n");
}

void h_pop() {
  StudentData it = {0};
  if (pop_item(&it) != 0) {
    printf("no items left\n");
    return;
  }

  sd_print(&it);
  sd_free_content(&it);
}

void h_find() {
  char phrase[255];
  printf("prompt searched field\navailable fields:\n");
  printf("\t0 - name\n");
  printf("\t1 - surname\n");
  printf("\t2 - birth_year\n");
  int field_id = ask_menu("field: ");
  if (field_id < 0 || field_id > 2) {
    printf("wrong menu option\n");
    return;
  }
  printf("searched phrase: ");
  scanf("%252s", phrase);

  printf("searched phrase:\t%s\n", phrase);
  printf("in field:\t%s\n", StudentDataFieldName[field_id]);

  StudentData it;
  if (search_item(&it, field_id, phrase) == 1) {
    printf("found:\n");
    sd_print(&it);
    return;
  }

  printf("no item found\n");
}

void h_write() {
  char path[255];
  printf("file path: ");
  scanf("%s", path);

  FILE *file = fopen(path, "wb");
  if (file == NULL) {
    printf("couldn't open/create file '%s'", path);
    return;
  }
  printf("using '%s'\n", path);

  write_items(file);
  fclose(file);
}

void h_read() {
  char path[255];
  printf("file path: ");
  scanf("%s", path);

  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    printf("couldn't open file '%s'", path);
    return;
  }
  printf("using '%s'\n", path);

  read_items(file);
  fclose(file);
}
