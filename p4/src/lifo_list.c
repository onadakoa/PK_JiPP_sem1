#include "student.h"
#include "tui.h"
#include <linux/limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _LI {
  StudentData *data;
  struct _LI *next;
} ListItem;

ListItem *head = NULL;

int add_item(StudentData *data);
StudentData *pop_item();
ListItem *search_item(StudentDataField field, char *value);
void print_items();
int count_items();
void free_items();
void free_item(ListItem **item);

int write_items(FILE *stream);
int read_items(FILE *stream);

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
      add_item(student);
    }
    fclose(file);
    printf("found %i items in %s\n\n", count_items(), path);
  }

  print_menu();
  int option = 0;
  while ((option = ask_menu("[LIFO l_list] ")) >= 0) {
    switch (option) {
    case INTERF_PUSH: {
      printf("creating StudentData Item:\n");
      StudentData *data = sd_ask();
      if (data == NULL) {
        printf("i won't add blank item\n");
        break;
      }
      add_item(data);
      printf("item added\n");
      break;
    }
    case INTERF_POP: {
      StudentData *data = pop_item();
      if (data == NULL)
        printf("no data left\n");
      else {
        sd_print(data);
        sd_free(&data);
      }
      break;
    }
    case INTERF_CLEAR:
      free_items();
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

  free_items();
  return 0;
}

int add_item(StudentData *data) {
  if (data == NULL)
    return -1;
  ListItem *item = xmalloc(sizeof(ListItem));
  item->data = data;
  item->next = head;

  head = item;
  return 0;
}

StudentData *pop_item() {
  if (head == NULL)
    return NULL;
  ListItem *item = head;
  head = head->next;
  StudentData *data = item->data;
  free(item);

  return data;
}

int count_items() {
  int i = 0;
  ListItem *tmp = head;
  while (tmp != NULL) {
    i++;
    tmp = tmp->next;
  }
  return i;
}

void print_items() {
  ListItem *tmp = head;
  int i = 0;
  while (tmp != NULL) {
    printf("%i: ", i);
    sd_print_horizontal(tmp->data, ", ");
    printf("\n");
    tmp = tmp->next;
    i++;
  }
}

ListItem *search_item(StudentDataField field, char *value) {
  ListItem *tmp = head;
  while (tmp != NULL) {
    char *fields[3] = {tmp->data->name, tmp->data->surname,
                       tmp->data->birth_year};

    if (strcmp(value, fields[field]) == 0) {
      return tmp;
    }
    tmp = tmp->next;
  }
  return NULL;
}

void free_items() {
  while (head != NULL) {
    ListItem *next = head->next;
    sd_free(&head->data);
    free(head);
    head = next;
  }
}

void h_find() {
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
  char *phrase = line_input();
  if (phrase == NULL) {
    printf("cancelled\n");
    return;
  }

  printf("searched phrase:\t%s\n", phrase);
  printf("in field:\t%s\n", StudentDataFieldName[field_id]);

  ListItem *found_item = search_item(field_id, phrase);
  free(phrase);
  if (found_item != NULL) {
    printf("found:\n");
    sd_print(found_item->data);
    return;
  }

  printf("no item found\n");
}

void h_write() {
  printf("file path: ");
  char *path = line_input();
  if (path == NULL) {
    printf("cancelled\n");
    return;
  }

  FILE *file = fopen(path, "wb");
  if (file == NULL) {
    printf("couldn't write to '%s'\n", path);
    free(path);
    return;
  }
  printf("using '%s'\n", path);

  write_items(file);
  free(path);
  fclose(file);
}

int write_items(FILE *stream) {
  if (stream == NULL)
    return -1;

  ListItem *tmp = head;
  while (tmp != NULL) {
    char *fields[3] = {tmp->data->name, tmp->data->surname,
                       tmp->data->birth_year};
    for (int i = 0; i < 3; i++) {
      size_t len = strlen(fields[i]);

      fwrite(&len, sizeof(size_t), 1, stream);
      fwrite(fields[i], 1, len, stream);
    }

    tmp = tmp->next;
  }
  return 0;
}

void h_read() {
  if (head != NULL) {
    printf("your stack isn't empty. do you want to erase it? [y/n]: ");
    char *inp = line_input();
    if (inp == NULL)
      return;
    char option = inp[0];
    free(inp);
    if (option != 'y')
      return;
    free_items();
  }

  printf("file path: ");
  char *path = line_input();
  if (path == NULL) {
    printf("cancelled\n");
    return;
  }

  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    printf("couldn't open '%s'\n", path);
    free(path);
    return;
  }
  printf("using '%s'\n", path);

  read_items(file);
  free(path);
  fclose(file);
  printf("new count: %i\n", count_items());
}

int read_items(FILE *stream) {
  if (stream == NULL)
    return 1;
  if (head != NULL)
    return 2;

  ListItem **tmp_head = &head;

  while (1) {
    StudentData *data = calloc(1, sizeof(StudentData));
    if (!data) {
      printf("can't calloc\n");
      exit(1);
    }
    char **fields[3] = {&data->name, &data->surname, &data->birth_year};
    for (int i = 0; i < 3; i++) {
      char *str = sd_read_fstring(stream);
      if (str == NULL) {
        sd_free(&data);

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

    ListItem *item = xmalloc(sizeof(ListItem));
    item->data = data;
    item->next = NULL;

    if (*tmp_head == NULL) {
      *tmp_head = item;
    } else {
      (*tmp_head)->next = item;
      tmp_head = &item->next;
    }
  }
  return 0;
}
