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
void free_item(ListItem **item);

int write_items(FILE *stream);
int read_items(FILE *stream);

void h_exit();
void h_count();
void h_pop();
void h_add();
void h_find();

void h_read();
void h_write();

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
  while ((option = ask_menu("[FIFO l_list] ")) >= 0) {
    switch (option) {
    case INTERF_PUSH:
      h_add();
      break;
    case INTERF_POP:
      h_pop();
      break;
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

  return 0;
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

void print_items() {
  int i = 0;
  ListItem *tmp = head;
  while (tmp != NULL) {
    printf("%i: ", i);
    sd_print_horizontal(tmp->data, "\t");
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

void free_item(ListItem **item) {
  sd_free(&(*item)->data);
  free(*item);
  (*item) = NULL;
}

int write_items(FILE *stream) {
  if (stream == NULL)
    return 1;

  ListItem *tmp = head;
  while (tmp != NULL) {
    StudentData *data = tmp->data;

    char *fields[3] = {data->name, data->surname, data->birth_year};
    for (int i = 0; i < 3; i++) {
      int len = strlen(fields[i]);

      fwrite(&len, sizeof(int), 1, stream);
      fwrite(fields[i], 1, len, stream);
    }

    tmp = tmp->next;
  }

  return 0;
}

int read_items(FILE *stream) {
  if (stream == NULL)
    return 1;
  if (head != NULL)
    return 2;

  ListItem **tmp_head = &head;

  while (1) {
    StudentData *data = calloc(1, sizeof(StudentData));
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

    ListItem *item = malloc(sizeof(ListItem));
    item->data = data;
    item->next = NULL;

    if (*tmp_head == NULL) {
      *tmp_head = item;
    } else {
      (*tmp_head)->next = item;
      tmp_head = &(*tmp_head)->next;
    }
  }

  return 0;
}

// TUI HANDLERS
void h_exit() {
  free_items();
  // tui_stop();
}

void h_count() { printf("items count: %i\n", count_items()); }

void h_pop() {
  ListItem *item = pop_item();
  if (item == NULL) {
    printf("couldn't pop item\n");
    return;
  }

  sd_print_horizontal(item->data, "\t");
  printf("\n");

  free_item(&item);
}

void h_add() {
  printf("\ncreating StudentData Item:\n");
  StudentData *data = sd_ask();

  add_item(data);
  printf("item added");
  // tui_clear_input_buffer();
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

  ListItem *found_item = search_item(field_id, phrase);
  if (found_item != NULL) {
    printf("found:\n");
    sd_print(found_item->data);
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
  if (head != NULL) {
    printf("your stack isn't empty. do you want to erase it? [y/n]: ");
    char option[4];
    scanf("%s", option);

    if (option[0] != 'y')
      return;
    free_items();
  }
  char path[255];
  printf("file path: ");
  scanf("%s", path);

  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    printf("couldn't open/create file '%s'", path);
    return;
  }
  printf("using '%s'\n", path);

  if (read_items(file) == 0) {
    printf("new count: %i\n", count_items());
  }
  fclose(file);
}
