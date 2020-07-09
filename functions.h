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

double setDensity() 
{
  double density;

  std::string string;
  std::string::size_type sz;

  std::cout << "Insert the population density (between 0.0 and 1.0): ";
  std::cin >> string;
  density = std::stod(string, &sz);
  while (1) {
    if (checkComma(string)) {
      density = std::stod(string, &sz) + 0.1 * std::stod(string.substr(1+sz));
    }
    if (density <= 0 || density > 1) {
      std::cout << "The density value must be between 0.0 and 1.0. Insert it again: ";
      std::cin >> string;
    } else { break;}
    density = std::stod(string, &sz);
  }
  return density;
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

double setPInf ()
{

  double pInf;
  
  std::string string;
  std::string::size_type sz;

  std::cout << "Insert the probability of infection due to a direct contact "
               "(between 0.0 and 1.0): ";
  std::cin >> string;
  pInf = std::stod(string, &sz);
  while (1) {
    if (checkComma(string)) {
      pInf = std::stod(string, &sz) + 0.1 * std::stod(string.substr(1+sz));
    }
    if (pInf <= 0 || pInf > 1) {
      std::cout << "The percentage value must be between 0.0 and 1.0. Insert it "
                 "again: ";
      std::cin >> string;
    } else { break; }
    pInf = std::stod(string, &sz);
  }
  return pInf;
}

double setTMean ()
{

  double tMean;
  
  std::string string;
  std::string::size_type sz;

  std::cout << "Insert the average time of recovery (between 1 and 40 days): ";
  std::cin >> string;
  tMean = std::stod(string, &sz);
  while (1) {
    if (checkComma(string)) {
      tMean = std::stod(string, &sz) + 0.1 * std::stod(string.substr(1+sz));
    }
    if (tMean < 1 || tMean > 40) {
      std::cout << "Insert a plausible value of healing time: ";
      std::cin >> string;
    } else { break; }
    tMean = std::stod(string, &sz);
  }
  return tMean;
}

double setRange ()
{

  double range;
  
  std::string string;
  std::string::size_type sz;

  std::cout << "Insert the range of infection (1 is direct contact. A "
               "plausible value could be between 1 and 5): ";
  std::cin >> string;
  range = std::stod(string, &sz);
  while (1) {
    if (checkComma(string)) {
      range = std::stod(string, &sz) + 0.1 * std::stod(string.substr(1+sz));
    }
    if (range < 1 || range > 5) {
      std::cout << "Insert a plausible value of infection's range: ";
      std::cin >> string;
    } else { break; }
    range = std::stod(string, &sz);
  }
  return range;
}


#endif /* functions_h */
