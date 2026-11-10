#include <project.h>
#include <string.h>

ProMode pro_mode = FIFO;
ProType pro_type = ARRAY;
const char *pro_mode_name[] = {"LIFO", "FIFO"};
const char *pro_type_name[] = {"array", "l_list"};

char pro_prefix[15] = "[FIFO l_list] ";

void pro_update_prefix() {
  char n_prefix[15];
  strcpy(n_prefix, "[");
  strcat(n_prefix, pro_mode_name[pro_mode]);
  strcat(n_prefix, " ");
  strcat(n_prefix, pro_type_name[pro_type]);
  strcat(n_prefix, "] ");

  strcpy(pro_prefix, n_prefix);
}

int pro_set_mode_type(char *mt) {
  if (!strcmp(mt, "LIFO") || !strcmp(mt, "lifo")) {
    pro_mode = LIFO;
    return 1;
  } else if (!strcmp(mt, "FIFO") || !strcmp(mt, "fifo")) {
    pro_mode = FIFO;
    return 1;
  } else if (!strcmp(mt, "ARRAY") || !strcmp(mt, "array")) {
    pro_type = ARRAY;
    return 2;
  } else if (!strcmp(mt, "LIST") || !strcmp(mt, "list") ||
             !strcmp(mt, "linked_list") || !strcmp(mt, "LINKED_LIST")) {
    pro_type = LINKED_LIST;
    return 2;
  }

  return 0;
}
