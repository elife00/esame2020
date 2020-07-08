#include "functions.h"
#include "epidemic.hpp"

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include <thread>

// g++ -std=c++17 gui.cpp -o gui -lsfml-graphics -lsfml-window -lsfml-system
// g++ -std=c++17 functions.test.cpp -o gui
// mkdir build
// cd build
// cmake -DCMAKE_BUILD_TYPE=Debug ..
// make
//./epidemic-sfml
// dopo la prima volta bastano gli ultimi due

int main() {
  int dim = 120;
  int quadSize = 5;
  double pInf;
  double percInf;
  int tMean;
  double density;
  int range;

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
  
  percInf = setPercInf();

  std::cout << "Insert the probability of infection due to a direct contact "
               "(between 0.0 and 1.0: ";
  std::cin >> pInf;
  while (pInf <= 0 || pInf > 1) {
    std::cout << "The percentage value must be between 0.0 and 1.0. Insert it "
                 "again: ";
    std::cin >> pInf;
  }
  std::cout << "Insert the average time of recovery (between 1 and 40 days): ";
  std::cin >> tMean;
  while (tMean < 1 || tMean > 40) {
    std::cout << "Insert a plausible value of healing time: ";
    std::cin >> tMean;
  }
  std::cout << "Insert the range of infection (1 is direct contact. A "
               "plausible value could be between 1 and 5): ";
  std::cin >> range;

  /*sf::RenderWindow epidemicWindow(
      sf::VideoMode(sf::VideoMode::getDesktopMode().height * 2 / 3,
                    sf::VideoMode::getDesktopMode().height * 2 / 3),
      "My epidemic");*/
    sf::RenderWindow epidemicWindow(sf::VideoMode(dim * quadSize + 100, dim * quadSize + 100),"My epidemic");
  epidemicWindow.setVerticalSyncEnabled(true);

  // change the position of the window (relatively to the desktop)
  epidemicWindow.requestFocus();
  epidemicWindow.setPosition(sf::Vector2i(
      (sf::VideoMode::getDesktopMode().width - epidemicWindow.getSize().x) / 2,
      (sf::VideoMode::getDesktopMode().height - epidemicWindow.getSize().x) / 2));

  Board population(dim, density);
  population.infection(percInf);

  // run the program as long as the window is open
  while (epidemicWindow.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (epidemicWindow.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        epidemicWindow.close();
      }
    }

    auto rappresentation = population.Draw(quadSize);
    rappresentation.setPosition(epidemicWindow.getSize().x / 2,
                                epidemicWindow.getSize().y / 2);

    epidemicWindow.clear(sf::Color::Black);
    epidemicWindow.draw(rappresentation);
    epidemicWindow.display();

    if (population.current_situation().i == 0) {
      std::this_thread::sleep_for(std::chrono::seconds(3));
      epidemicWindow.close();
      population.trend();

      system("root");
    }

    population = population.epidemic_range(pInf, tMean, range);
    // population = population.epidemic2(pInf,1./tMean);

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
  return 0;
}
