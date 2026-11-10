#include <stdio.h>
#include <stdlib.h>
#include <project.h>
#include <student.h>
#include <tui.h>

void h_exit_tui() { tui_stop(); }
void h_mode_tui(int argc, char **argv);

int main(int argc, char **argv) {
  char path[] = "dane.txt";

  FILE *file = fopen(path, "r");
  if (file == NULL) {
    return 1;
  }

  StudentData *st;
  while ((st = fsd_read_one(file)) != NULL) {
    sd_print_horizontal(st, '\t');
    printf("\n");
    free(st);
  }

  tui_add_handler(h_exit_tui, "exit", "exit");
  tui_add_handler(h_mode_tui, "mode", "sets mode");

  while (tui_should_run()) {
    char *input = tui_pask(pro_prefix);

    char **m_argv = NULL;
    int m_argc = 0;
    tui_parse(input, &m_argc, &m_argv);

    if (m_argc > 0) {
      char *trigger = tui_argv_pull(&m_argc, &m_argv);

      if (tui_execute(trigger, m_argc, m_argv) == 0) {
        tui_print_handlers();
      }

      free(trigger);
    } else {
      tui_print_handlers();
    }

    free(input);
    tui_argv_free(m_argc, &m_argv);
  }
  tui_clean();

  return 0;
}

void h_mode_tui(int argc, char **argv) {
  if (argc == 0) {
    printf("no arguments\n");
    return;
  }
  for (int i = 0; i < argc; i++) {
    switch (pro_set_mode_type(argv[i])) {
    case 1:
      printf("project mode set to: %s\n", pro_mode_name[pro_mode]);
      break;
    case 2:
      printf("project type set to: %s\n", pro_type_name[pro_type]);
      break;

    case 0:
      printf("failed to use %s\n", argv[i]);
      break;
    };
  }
  pro_update_prefix();
}
