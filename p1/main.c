#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

double f_scisle(double x) { return x * cos(x); }

// a_(n+1)/a_n
double f_szereg(double x, double delta, int M, int *lws, int *ws) {
  double sum = 0;
  double term = x;

  for (int n = 0; n < M; n++) {
    sum += term;

    if (fabs(term) < delta) {
      *lws = n + 1;
      *ws = (n == M - 1) ? 3 : 1;
      return sum;
    }

    term *= -x * x / ((2.0 * n + 1.0) * (2.0 * n + 2.0));
  }

  *lws = M;
  *ws = 2;
  return sum;
}

int main() {
  printf("przedzial [a,b]:\n");
  printf("a: ");
  double a = 0;
  scanf("%lf", &a);
  printf("b: ");
  double b = 0;
  scanf("%lf", &b);
  if (a > b) {
    printf("niepoprawny przedzial [%lf, %lf]\n", a, b);
    return 1;
  }
  printf("liczba części(n): ");
  int n = 0;
  scanf("%i", &n);
  if (n < 1) {
    printf("niepoprawne podzial na czesci (n>0)\n");
    return 1;
  }
  printf("dokładność (delta): ");
  double d = 0;
  scanf("%lf", &d);
  if (d <= 0) {
    printf("delta musi byc > 0\n");
    return 1;
  }
  printf("maksymalna ilość summowanych wyrazów (M): ");
  int m = 0;
  scanf("%i", &m);
  if (m < 1) {
    printf("niepoprawna ilosc sumowanych wyrazow (m>0)\n");
    return 1;
  }

  printf("plik do zapisu: ");
  char *path = malloc(255);
  if (!path) {
    printf("blad allokacji\n");
    exit(1);
  }

  scanf("%250s", path);
  FILE *file = fopen(path, "w");
  free(path);
  if (file == NULL) {
    printf("nie moge otworzyc pliku\n");
    return 1;
  }

  printf("x\tf_szereg\tf_scisle\tliczba wyrazow\twarunek stopu\n");
  fprintf(file, "x\tf_szereg\tf_scisle\tliczba wyrazow\twarunek stopu\n");
  double diff = (b - a) / n;
  for (int i = 0; i <= n; i++) {
    double x = a + i * diff;

    double scisle = f_scisle(x);
    int lws = 0; // liczba wyrazow uzyta
    int ws = 0; // warunek stopu
    double szereg = f_szereg(x, d, m, &lws, &ws);

    printf("%g\t%lf\t%lf\t%i\t%i\n", x, szereg, scisle, lws, ws);
    fprintf(file, "%g\t%lf\t%lf\t%i\t%i\n", x, szereg, scisle, lws, ws);
  }
  fclose(file);
  return 0;
}
