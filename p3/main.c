#include <stdlib.h>
#define matrix_size(n) ((n * (n + 1)) / 2)
#include <stdio.h>

void read_matrix(int n, double *matrix);
void read_vector(int n, double *vector);

void print_matrix(int n, double *matrix);
void print_vector(int n, double *vector);

double *multiply_matrix_vector(int n, double *matrix, double *vector);

int main() {

  printf("podaj rozmiar macierza n: ");
  int n = 0;
  scanf("%i", &n);
  if (n < 2) {
    printf("rozmiar musi być >= 2");
    return 1;
  }

  int size = matrix_size(n);

  double matrix[size];
  read_matrix(n, matrix);
  print_matrix(n, matrix);

  double vector[n];
  read_vector(n, vector);
  print_vector(n, vector);

  double *out_vector = multiply_matrix_vector(n, matrix, vector);
  printf("\nAx:\n");
  print_vector(n, out_vector);

  free(out_vector);
  return 0;
}

void read_matrix(int n, double *matrix) {
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

/*           i
 * [ 1 2 3 ] 1
 * [ x 4 5 ] 2
 * [ x x 8 ] 3
 * j 1 2 3
 * a[3,1] = 3 [x]
 * a[1,3] = 3
 */
double *matrix_get_element(int n, double *matrix, int i, int j) {
  if (j < i) {
    return matrix_get_element(n, matrix, j, i);
  }

  int offset = i * n - (i * (i - 1)) / 2;

  return &matrix[offset + (j - i)];
}

void print_matrix(int n, double *matrix) {
  printf("\nzbudowany matrix:\n");
  for (int i = 0; i < n; i++) {
    printf("|\t");
    for (int k = 0; k < n; k++) {
      printf("%g\t", *matrix_get_element(n, matrix, i, k));
    }

    printf("|\n");
  }
}

void print_vector(int n, double *vector) {
  printf("\nzbudowany vector:\n[\t");
  for (int i = 0; i < n; i++) {
    printf("%g\t", vector[i]);
  }
  printf("]\n");
}

double *multiply_matrix_vector(int n, double *matrix, double *vector) {
  double *out_vector = malloc(sizeof(double) * n);

  for (int i = 0; i < n; i++) {
    double out_element = 0;
    for (int k = 0; k < n; k++) {
      out_element += (*matrix_get_element(n, matrix, i, k)) * vector[k];
    }
    out_vector[i] = out_element;
  }

  return out_vector;
}
