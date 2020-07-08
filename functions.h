#ifndef functions_h
#define functions_h

#include <cassert>
#include <cmath>
#include <string>
#include <iostream>

inline long double factorial(int n)
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

inline long double cumulative(int k, int mu)
{
  assert(k > 0);  // forse Ã¨ meglio lanciare un messaggio in output se Ã¨ un
                  // double o 0
  long double p = 0;
  for (int i = 0; i <= k; ++i) {
    p += (pow(mu, i) * exp(-mu) / factorial(i));
  }
  return p;
}


inline bool checkComma (std::string string) 
{
    for (auto v : string)
    {
        if (v == ',') { return true; }
    }
    return false;
}

inline double setPercInf ()
{

  double percInf;
  
  std::string string;
  std::string::size_type sz;

  std::cout << "Insert the initial percentage of infected (between 0.0 and 1.0): ";
  std::cin >> string;
  percInf = std::stod(string, &sz);
  while (1) {
    if (checkComma(string)) {
      percInf = std::stod(string, &sz) + 0.1 * std::stod(string.substr(1+sz));
    }
    if (percInf <= 0 || percInf > 1) {
      std::cout << "The percentage value must be between 0.0 and 1.0. Insert it "
                   "again: ";
      std::cin >> string;
    } else { break; }
    percInf = std::stod(string, &sz);
  }
  return percInf;
}

#endif /* functions_h */
