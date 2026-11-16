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

void h_exit();
void h_count();
void h_pop();
void h_add();
void h_find(int argc, char **argv);

void h_read(int argc, char **argv);
void h_write(int argc, char **argv);

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

  tui_add_handler(h_exit, "exit",
                  "zakonczenie programu, usuniecie wszytkich elementow");
  tui_add_handler(h_count, "count",
                  "sprawdzanie liczby elementow w strukturze");
  tui_add_handler(h_pop, "pop", "pobranie elementu (oraz usuniecie go)");
  tui_add_handler(h_add, "add", "dodawanie elementu");

  tui_add_handler(
      h_find, "find",
      "wyszukanie konkretnego elementu po okreslonym polu (NAME, "
      "SURNAME, BIRTH_YEAR)\n\t\tUsage: find <field_name> <phrase>");

  tui_add_handler(print_items, "print", "wypisanie wszystkich elementow");
  tui_add_handler(free_items, "free", "usuniecie calej struktury");

  tui_add_handler(
      h_write, "write",
      "zapis struktury do pliku binarnego\n\t\tUsage: write <path>");

  while (tui_should_run()) {
    char *input = tui_pask("[FIFO l_list] ");
    int m_argc = 0;
    char **m_argv = NULL;
    tui_parse(input, &m_argc, &m_argv);
    free(input);
    input = NULL;

    if (m_argc > 0) {
      char *trigger = tui_argv_pull(&m_argc, &m_argv);
      if (!tui_execute(trigger, m_argc, m_argv)) {
        printf("couldn't recognise '%s'\n\n", trigger);
        tui_print_handlers();
      }
      free(trigger);
    } else {
      tui_print_handlers();
    }
    tui_argv_free(&m_argc, &m_argv);
  }

  tui_clean();
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

// TUI HANDLERS
void h_exit() {
  free_items();
  tui_stop();
}

void h_count() { printf("items count: %i\n", count_items()); }

void h_pop() {
  ListItem *item = pop_item();

  sd_print_horizontal(item->data, "\t");
  printf("\n");

  free_item(&item);
}

void h_add() {
  printf("\ncreating StudentData Item:\n");
  StudentData *data = sd_ask();

  add_item(data);
  printf("item added");
  tui_clear_input_buffer();
}

void h_find(int argc, char **argv) {
  if (argc <= 1) {
    printf("not enough arguments");
    return;
  }
  char *field_name = argv[0];
  argc--;
  argv = &argv[1];

  int field_id = -1;
  for (int i = 0; i < 3; i++) {
    if (strcmp(StudentDataFieldName[i], field_name) == 0) {
      field_id = i;
      break;
    }
  }
  if (field_id == -1) {
    printf("couldn't recognise '%s' as field", field_name);
    return;
  }

  int final_length = argc;
  for (int i = 0; i < argc; i++) {
    final_length += strlen(argv[i]);
  }

  char *phrase = malloc(final_length);
  strcpy(phrase, "");
  for (int i = 0; i < argc; i++) {
    strcat(phrase, argv[i]);
    if (i < argc - 1)
      strcat(phrase, " ");
  }

  printf("searched phrase:\t%s\n", phrase);
  printf("in field:\t%s\n", StudentDataFieldName[field_id]);

  ListItem *found_item = search_item(field_id, phrase);
  if (found_item != NULL) {
    printf("found:\n");
    sd_print(found_item->data);
    free(phrase);
    return;
  }

  printf("no item found\n");
  free(phrase);
}

void h_write(int argc, char **argv) {
  if (argc != 1) {
    printf("not enough arguments\n");
    return;
  }
  char *path = argv[0];
  FILE *file = fopen(path, "wb");
  if (file == NULL) {
    printf("couldn't open/create file '%s'", path);
    return;
  }
  printf("using '%s'\n", path);

  write_items(file);
}
