#include <stdio.h>
#include <stdlib.h>
#include <student.h>
#include <tui.h>

void test() { printf("something beautifull\n"); }
void fn2(int argc, char **argv) { printf("fn2 function\n"); }

int main() {
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

  tui_add_handler(test, "test", "function that prints 'something beautifull'");
  tui_add_handler(fn2, "fn2", "function with argc and argv'");

  while (tui_should_run()) {
    char *input = tui_ask();

    char **argv = NULL;
    int argc = 0;
    tui_parse(input, &argc, &argv);

    if (argc > 0) {
      char *trigger = tui_argv_pull(&argc, &argv);

      if (tui_execute(trigger, argc, argv) == 0) {
        tui_print_handlers();
      }

      free(trigger);
    }

    free(input);
    tui_argv_free(argc, &argv);
  }

  return 0;
}
