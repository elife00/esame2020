#ifndef functions_h
#define functions_h

#include <cassert>
#include <cmath>
#include <string>
#include <iostream>
#include <array>

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


//the order is density, percInf, pInf, tMean, range

inline std::array<double,5> input_parameters () {
    
    std::array<double,5> parameters;
    
    std::string out_density = "Insert the population density (between 0.0 and 1.0): ";
    std::string out_percInf = "Insert the initial percentage of infected among the population (between 0.0 and 1.0): ";
    std::string out_pInf = "Insert the probability of infection due to a contact (between 0.0 and 1.0): ";
    std::string out_tMean = "Insert the average time of recovery (between 1 and 40 days): ";
    std::string out_range = "Insert the range of infection (1 is direct contact. Plausible values could be between 1 and 5): ";
    
    std::array<std::string,5> output = {out_density, out_percInf, out_pInf, out_tMean, out_range};
    
    for (int i=0; i!=5; ++i){
        std::string string;
        std::string::size_type sz;
        std::cout << output[i] << '\n';
        std::cin >> string;
        
        parameters[i] = std::stod(string, &sz);
        while (1)
        {
            if (checkComma(string))
            {
                parameters[i] = std::stod(string, &sz) + 0.1 * std::stod(string.substr(1+sz));
            } else {
                parameters[i] = std::stod(string, &sz);
            }
            if (i<=2 && (parameters[i] <=0 || parameters[i] > 1))
            {
                std::cout << "Insert a value in the range 0-1 (percentage value): ";
                std::cin >> string;
            } else if (i==3 && (parameters[i] < 1 || parameters[i] > 40)) {
                std::cout << "Insert a value in the range 1-40 (days): ";
                std::cin >> string;
            } else if (i == 4 && (parameters[i] < 1 || parameters[i] > 10)){
                std::cout << "Insert a plausible value for the range of infection (could be between 1 and 5): ";
                std::cin >> string;
            } else { break; }
        }
    }
    return parameters;
    }
        


#endif /* functions_h */
