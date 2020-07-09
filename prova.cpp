#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "functions.h"
#include "sfml.h"
#include <iostream>
#include <string>

// g++ prova.cpp -o prova -lsfml-graphics -lsfml-window -lsfml-system

int main() {
  sf::RenderWindow prova(sf::VideoMode(200, 400), "My epidemic");
  prova.setPosition(
      sf::Vector2i(sf::VideoMode::getDesktopMode().width - prova.getSize().x,
      (sf::VideoMode::getDesktopMode().height - prova.getSize().y) / 2));

  std::array<sf::Text, 5> legend_ = legend();

    prova.clear(sf::Color::Black);
    prova.draw(legend_[0]);
    prova.draw(legend_[1]);
    prova.draw(legend_[2]);
    prova.draw(legend_[3]);
    prova.draw(legend_[4]);    
    prova.display();
  
}

/*
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
      std::cout << "The density value must be between 0.0 and 1.0. Insert it
again: "; std::cin >> string; } else { break;} density = std::stod(string, &sz);
  }
  return density;
}

std::cout << "Insert the initial percentage of infected (between 0.0 and 1.0):
"; std::cin >> percInf; while (percInf <= 0 || percInf > 1) { std::cout << "The
percentage value must be between 0.0 and 1.0. Insert it " "again: "; std::cin >>
percInf;
  }


double setPercInf ()
{

  double percInf;

  std::string string;
  std::string::size_type sz;

  std::cout << "Insert the initial percentage of infected (between 0.0 and 1.0):
"; std::cin >> string; percInf = std::stod(string, &sz); while (1) { if
(checkComma(string)) { percInf = std::stod(string, &sz) + 0.1 *
std::stod(string.substr(1+sz));
    }
    if (percInf <= 0 || percInf > 1) {
      std::cout << "The percentage value must be between 0.0 and 1.0. Insert it
" "again: "; std::cin >> string; } else { break; } percInf = std::stod(string,
&sz);
  }
  return percInf;
}

std::cout << "Insert the probability of infection due to a direct contact "
               "(between 0.0 and 1.0: ";
  std::cin >> pInf;
  while (pInf <= 0 || pInf > 1) {
    std::cout << "The percentage value must be between 0.0 and 1.0. Insert it "
                 "again: ";
    std::cin >> pInf;
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
      std::cout << "The percentage value must be between 0.0 and 1.0. Insert it
" "again: "; std::cin >> string; } else { break; } pInf = std::stod(string,
&sz);
  }
  return pInf;
}

std::cout << "Insert the average time of recovery (between 1 and 40 days): ";
  std::cin >> tMean;
  while (tMean < 1 || tMean > 40) {
    std::cout << "Insert a plausible value of healing time: ";
    std::cin >> tMean;
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

std::cout << "Insert the range of infection (1 is direct contact. A "
               "plausible value could be between 1 and 5): ";
  std::cin >> range;


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












/*
int main() {
  std::string den = "1,5";
  std::string::size_type sz;
  // = den.substr(0, sz);
  //std::cout << std::stod(den, &sz) << '.' << std::stod(den.substr(++sz)) <<
'\n'; std::string densi = std::to_string(std::stod(den, &sz) + 0.1 *
std::stod(den.substr(1+sz))); std::cout << densi << '\n' ;
}

/*
int main ()
{
  std::string orbits ("365.24 29.53");
  std::string::size_type sz;     // alias of size_t

  double earth = std::stod (orbits,&sz);
  double moon = std::stod (orbits.substr(sz));
  std::cout << "The moon completes " << (earth/moon) << " orbits per Earth
year.\n"; return 0;
}
*/