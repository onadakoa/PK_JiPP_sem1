#include "tui.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  EXIT = 0,
  RUN = 1,
} StatusCode;

StatusCode status = RUN;

static HandlerData **handlers_list = NULL;
static uint8_t handlers_list_size = 0;

void tui_add_handler(handler_fn_t handler, const char *trigger,
                     const char *description) {
  handlers_list_size++;

  handlers_list =
      realloc(handlers_list, sizeof(HandlerData *) * handlers_list_size);
  if (handlers_list == NULL)
    exit(1);

  HandlerData *data = malloc(sizeof(HandlerData));
  data->trigger = trigger;
  data->handler = handler;
  data->description = description;

  handlers_list[handlers_list_size - 1] = data;

  return;
}

void tui_print_handlers() {
  for (int i = 0; i < handlers_list_size; i++) {
    printf("%s\t-\t%s\n", handlers_list[i]->trigger,
           handlers_list[i]->description);
  }
}

int tui_execute(char *trigger, int argc, char **argv) {
  for (int i = 0; i < handlers_list_size; i++) {
    HandlerData *item = handlers_list[i];
    if (strcmp(item->trigger, trigger) == 0) {
      item->handler(argc, argv);
      return 1;
    }
  }

  return 0;
}

void tui_clean() {
  for (int i = 0; i < handlers_list_size; i++) {
    free(handlers_list[i]);
  }
  free(handlers_list);
  handlers_list = NULL;
  handlers_list_size = 0;
}

int tui_should_run() { return (int)status; }
void tui_stop() { status = EXIT; }

char *tui_pask(char *prefix) {
  printf("\n%s", prefix);
  char *out = malloc(INPUT_SIZE);
  if (fgets(out, INPUT_SIZE, stdin) == NULL) {
    free(out);
    return NULL;
  }

  return out;
}
char *tui_ask() { return tui_pask(DEFAULT_PREFIX); }

void tui_parse(char *s, int *argc, char ***argv) {
  (*argc) = 0;
  (*argv) = NULL;

  int len = strlen(s);

  int cur = 0;
  for (int i = 0; i <= len; i++) {
    if (s[i] == ' ' || s[i] == '\0' || s[i] == '\n') {
      int token_len = i - cur;

      if (token_len > 0) {

        char *token = malloc(token_len + 1);
        memcpy(token, s + cur, token_len);
        token[token_len] = '\0';

        (*argc)++;

        *argv = realloc(*argv, *argc * sizeof(char *));
        (*argv)[(*argc) - 1] = token;
      }
      cur = i + 1;
    }
  }
}

void tui_argv_free(int argc, char ***argv) {
  for (int i = 0; i < argc; i++) {
    free((*argv)[i]);
  }
  free((*argv));
  (*argv) = NULL;
}

char *tui_argv_pull(int *argc, char ***argv) {
  if (*argc < 1)
    return NULL;

  char *first = (*argv)[0];

  if (*argc == 1) {
    free(*argv);
    *argv = NULL;
    *argc = 0;
    return first;
  }

  for (int i = 1; i < *argc; i++) {
    (*argv)[i - 1] = (*argv)[i];
  }

  (*argc)--;
  (*argv) = realloc(*argv, sizeof(char *) * (*argc));

  return first;
}
