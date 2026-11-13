#ifndef TUI_H
#define TUI_H

#define DEFAULT_PREFIX "[ >] "
#define INPUT_SIZE 64

typedef void (*handler_fn_t)(int, char **);

typedef struct {
  handler_fn_t handler;
  const char *trigger;
  const char *description;
} HandlerData;

void tui_add_handler(handler_fn_t handler, const char *trigger,
                     const char *description);
int tui_execute(char *trigger, int argc, char **argv);

void tui_clean();
void tui_print_handlers();

int tui_should_run();
void tui_stop();
char *tui_ask();
char *tui_pask(char *prefix);
void tui_parse(char *s, int *argc, char ***argv);
void tui_argv_free(int *argc, char ***argv);
char *tui_argv_pull(int *argc, char ***argv);

void tui_clear_input_buffer();
#endif // !TUI
