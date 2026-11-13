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

void h_exit();
void h_count();

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

  tui_add_handler(print_items, "print", "wypisanie wszystkich elementow");
  tui_add_handler(free_items, "free", "usuniecie calej struktury");

  while (tui_should_run()) {
    char *input = tui_pask("[FIFO l_list] ");
    int m_argc = 0;
    char **m_argv = NULL;
    tui_parse(input, &m_argc, &m_argv);

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

// TUI HANDLERS
void h_exit() {
  free_items();
  tui_stop();
}

void h_count() { printf("items count: %i\n", count_items()); }
