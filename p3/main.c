#define MATRIX_SIZE(n) (n * (n + 1)) / 2
#include <stdio.h>

void read_matrix(int n, double *matrix);
void read_vector(int n, double *vector);

void print_matrix(int n, double *matrix);
void print_vector(int n, double *vector);

int main() {

  printf("Podaj rozmiar macierza n: ");
  int n = 0;
  scanf("%i", &n);
  if (n < 2) {
    printf("rozmiar musi być >= 2");
    return 1;
  }
  /*      n = 4
   *      [ * x x x ] [*xxx] 4
   *      [ x * x x ] [*xx] 3
   *      [ x x * x ] [*x] 2
   *      [ x x x * ] [*] 1       gsize = 1+2+3+4 = 10
   *      size = (n) + (n-1) + (n-2) + (n-3) + ... + 0
   *      size = (n*(n+1))/2
   */
  int size = MATRIX_SIZE(n);

  double matrix[size];
  read_matrix(n, matrix);
  print_matrix(n, matrix);

  double vector[n];
  read_vector(n, vector);
  print_vector(n, vector);

  return 0;
}

void read_matrix(int n, double *matrix) {
  int size = MATRIX_SIZE(n);

  int offset = 0;
  for (int i = 0; i < n; i++) {
    printf("wiersz %i (%i elementow): ", i, n - i);
    for (int k = 0; k < n - i; k++) {
      scanf("%lf", &matrix[offset + k]);
    }
    offset += n - i;
  }
}

void read_vector(int n, double *vector) {
  printf("dane dla wektora (%i elementow): ", n);
  for (int i = 0; i < n; i++) {
    scanf("%lf", &vector[i]);
  }
}

void print_matrix(int n, double *matrix) {
  int size = MATRIX_SIZE(n);

  printf("\nZbudowany matrix:\n");
  int offset = 0;
  for (int i = 0; i < n; i++) {
    printf("|\t");
    for (int k = 0; k < i; k++) {
      printf("\t");
    }
    for (int k = 0; k < n - i; k++) {
      printf("%g\t", matrix[offset + k]);
    }

    printf("|\n");
    offset += n - i;
  }
}

void print_vector(int n, double *vector) {
  printf("\nZbudowany vector:\n[\t");
  for (int i = 0; i < n; i++) {
    printf("%g\t", vector[i]);
  }
  printf("]\n");
}
