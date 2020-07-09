#ifndef functions_h
#define functions_h

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <string>

inline long double factorial(int n) {
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

inline long double cumulative(int k, int mu) {
  assert(k > 0); // forse Ã¨ meglio lanciare un messaggio in output se Ã¨ un
                 // double o 0
  long double p = 0;
  for (int i = 0; i <= k; ++i) {
    p += (pow(mu, i) * exp(-mu) / factorial(i));
  }
  return p;
}

inline bool checkComma(std::string string) {
  for (auto v : string) {
    if (v == ',') {
      return true;
    }
  }
  return false;
}

// the order is density, percInf, pInf, tMean, range

inline std::array<double, 5> input_parameters() {

  std::array<double, 5> parameters;

  std::string out_density =
      "Insert the population density (between 0.0 and 1.0): ";
  std::string out_percInf = "Insert the initial percentage of infected among "
                            "the population (between 0.0 and 1.0): ";
  std::string out_pInf = "Insert the probability of infection due to a contact "
                         "(between 0.0 and 1.0): ";
  std::string out_tMean =
      "Insert the average time of recovery (between 1 and 40 days): ";
  std::string out_range = "Insert the range of infection (1 is direct contact. "
                          "Plausible values could be between 1 and 5): ";

  std::array<std::string, 5> output = {out_density, out_percInf, out_pInf,
                                       out_tMean, out_range};

  for (int i = 0; i != 5; ++i) {
    std::string string;
    std::string::size_type sz;
    std::cout << output[i] << '\n';
    std::cin >> string;

    parameters[i] = std::stod(string, &sz);
    while (1) {
      if (checkComma(string)) {
        parameters[i] =
            std::stod(string, &sz) + 0.1 * std::stod(string.substr(1 + sz));
      } else {
        parameters[i] = std::stod(string, &sz);
      }
      if (i <= 2 && (parameters[i] <= 0 || parameters[i] > 1)) {
        std::cout << "Insert a value in the range 0-1 (percentage value): ";
        std::cin >> string;
      } else if (i == 3 && (parameters[i] < 1 || parameters[i] > 40)) {
        std::cout << "Insert a value in the range 1-40 (days): ";
        std::cin >> string;
      } else if (i == 4 && (parameters[i] < 1 || parameters[i] > 10)) {
        std::cout << "Insert a plausible value for the range of infection "
                     "(could be between 1 and 5): ";
        std::cin >> string;
      } else {
        break;
      }
    }
  }
  return parameters;
}

inline std::array<double, 5> random_parameters() {
    std::array<double, 5> parameters;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_real_distribution<> d(0, 0.5);
    std::uniform_real_distribution<> perc(0, 0.2);
    std::uniform_int_distribution<> t(5, 30);
    std::uniform_int_distribution<> r(1, 3);
    
    //Despite its very low probability, adding control if the genereted value is 0
    parameters[0] = d(gen);
    if (parameters[0] == 0) {
        parameters[0] = d(gen);
    }
    parameters[1] = perc(gen);
    if (parameters[1] == 0) {
           parameters[1] = d(gen);
       }
    parameters[2] = perc(gen);
    if (parameters[2] == 0) {
        parameters[2] = d(gen);
    }
    parameters[3] = t(gen);
    parameters[4] = r(gen);

  std::cout << "Epidemic's parameters: " << '\n'
            << "population's density:  " << parameters[0] << '\n'
            << "initial percentage of infected among the population:  "
            << parameters[1] << '\n'
            << "probability of infection due to a contact:  " << parameters[2]
            << '\n'
            << "average time of recovery:  " << parameters[3] << '\n'
            << "range of infection:  " << parameters[4] << '\n';

  return parameters;
}

inline bool input_quarantine() {
  char ans2;
  bool quarantine = true;
  std::cout << "Quarantine option? (Y/N): ";
  std::cin >> ans2;
  while (ans2 != 'Y' && ans2 != 'y' && ans2 != 'N' && ans2 != 'n') {
    std::cout << "Invalid answer. Enter Y or N : ";
    std::cin >> ans2;
  }
  if (ans2 == 'y' || ans2 == 'Y') {
    quarantine = true;
  }
  if (ans2 == 'n' || ans2 == 'N') {
    quarantine = false;
  }
  return quarantine;
}

#endif /* functions_h */
