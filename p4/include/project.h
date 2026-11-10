#ifndef PROJEKT_H
#define PROJEKT_H

typedef enum {
  LIFO = 0,
  FIFO = 1,
} ProMode;
typedef enum {
  ARRAY = 0,
  LINKED_LIST = 1,
} ProType;

extern ProMode pro_mode;
extern ProType pro_type;
extern const char *pro_mode_name[];
extern const char *pro_type_name[];

extern char pro_prefix[15];
void pro_update_prefix();

/**
 * returns:
 * - 0 - when failed
 * - 1 - when changed 'pro_mode'
 * - 2 - when changed 'pro_type'
 *
 * accepts:
 * - 'lifo' 'LIFO' - sets mode to LIFO
 * - 'fifo' 'FIFO' - sets mode to FIFO
 * - 'array' 'ARRAY' - sets mode to ARRAY
 * - 'list' 'LIST' - sets mode to LINKED_LIST
 */
int pro_set_mode_type(char *mt);

#endif // !PROJEKT_H
