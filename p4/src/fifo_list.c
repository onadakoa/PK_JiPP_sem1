/*
 * fifo:
 * new items will be added at the tail of the list.
 * on item removal, the head will be removed first
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <student.h>
#include <tui.h>

typedef struct _LI {
  StudentData *data;
  struct _LI *next;
} ListItem;

ListItem *head = NULL;

int add_item(StudentData *data);
ListItem *pop_item();
ListItem *search_item(StudentDataField field, char *value);
void print_items();
int count_items();
void free_items();

int main(int argc, char **argv) {
  if (argc > 1) {
    char *path = argv[1];
    FILE *file = fopen(path, "r");
    if (file == NULL) {
      printf("couldn't open '%s'", path);
      return 1;
    }

    StudentData *student = NULL;
    while ((student = fsd_read_one(file)) != NULL) {
      add_item(student);
    }
    fclose(file);
  }
  printf("count: %i\n", count_items());
  ListItem *item = pop_item();
  if (item != NULL)
    sd_print(item->data);
  printf("count: %i\n", count_items());
}

int add_item(StudentData *data) {
  ListItem *item = malloc(sizeof(ListItem));
  item->data = data;
  item->next = NULL;
  if (head == NULL) {
    head = item;
    return 0;
  }

  int i = 1;
  ListItem *tmp = head;
  while (tmp->next != NULL) {
    tmp = tmp->next;
    i++;
  }
  tmp->next = item;
  return i;
}

ListItem *pop_item() {
  ListItem *out = head;
  if (head != NULL)
    head = head->next;
  return out;
}

// TODO: will change
void print_items() {
  ListItem *tmp = head;
  while (tmp != NULL) {
    sd_print_horizontal(tmp->data, ',');
    printf("\n");
    tmp = tmp->next;
  }
}

ListItem *search_item(StudentDataField field, char *value) {
  ListItem *tmp = head;
  while (tmp != NULL) {
    char *fields[3] = {tmp->data->name, tmp->data->surname,
                       tmp->data->birth_year};

    if (strcmp(fields[field], value) == 0)
      return tmp;

    tmp = tmp->next;
  }
  return NULL;
}

int count_items() {
  ListItem *tmp = head;
  int i = 0;
  while (tmp != NULL) {
    i++;
    tmp = tmp->next;
  }
  return i;
}

void free_items() {
  while (head != NULL) {
    ListItem *tmp = head->next;

    sd_free(&head->data);
    free(head);

    head = tmp;
  }
}
