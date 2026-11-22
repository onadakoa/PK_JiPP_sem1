#ifndef UTILS_H
#define UTILS_H

void error(char *format, ...);
void help();
int *argv_to_int(int argc, char **argv);
int check_if_in_array(int *array, int array_length, int item);

#endif // !UTILS_H
