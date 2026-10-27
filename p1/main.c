#include <bits/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double fact(size_t x) {
  double out = 1;
  for (size_t i = 1; i <= x; i++) {
    out *= i;
  }
  return out;
}

double x_cos_x(double x, size_t n) {
  double out = 0;

  for (size_t i = 0; i < n; i++) {
    out += pow(-1, i) / fact(2 * i) * pow(x, 2 * i + 1);
  }

  return out;
}

/*
 * optimized formula:
 * ai * -x^2/((2i+1)(2i+2))
 */
double x_cos_x_opt(double x, size_t n) {
  double sum = 0;
  double last_calc = x;

  for (size_t i = 0; i < n; i++) {
    sum += last_calc;
    last_calc *= (-1 * x * x) / ((2.0 * i + 1.0) * (2.0 * i + 2.0));
  }

  return sum;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("not enough arguments\n");
    printf("Usage:\n\t./main <x> <n>");
    return 0;
  }
  double x = atof(argv[1]);
  int n = atoi(argv[2]);
  if (n < 1) {
    printf("Error: n must be 1 or more");
    return 1;
  }

  printf("calculating xcosx for n: %i iterations\n", n);
  printf("x = %.17g\tn = %i\n", x, n);

  printf("xcosx:          \t%.17g\n", x_cos_x(x, n));
  printf("xcosx optimized:\t%.17g\n", x_cos_x_opt(x, n));

  return 0;
}
