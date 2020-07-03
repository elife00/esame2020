#ifndef functions_h
#define functions_h

#include <cassert>
#include <cmath>

long double factorial(int n)
{
  if (n == 0) {
    return 1;
  }
  long double factorial = 1;
  for (int i = 1; i <= n; ++i) {
    factorial *= i;
  }
  assert(factorial > 0);
  return factorial;
}

long double cumulative(int k, int mu)
{
  assert(k > 0);  // forse Ã¨ meglio lanciare un messaggio in output se Ã¨ un
                  // double o 0
  long double p = 0;
  for (int i = 0; i <= k; ++i) {
    p += (pow(mu, i) * exp(-mu) / factorial(i));
  }
  return p;
}

#endif /* functions_h */
