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
constexpr int dim = 120;
constexpr int quadSize = 5;

int main() {
    
    double density;
    double pInf;
    double percInf;
    int tMean;
    int range;
    std::array<double,5> parameters;
    
    std::cout << "Do you want to enter the epidemic's parameters? (Otherwise will be produced a random epidemic). (Y/N): " << '\n';
    char ans;
    std::cin >> ans;
    while (ans != 'Y' && ans != 'y' && ans != 'N' && ans != 'n'){
        std::cout << "Invalid answer. Enter Y or N : ";
        std::cin >> ans;
    }
    if (ans == 'y' || ans == 'Y')
    {
     parameters = input_parameters ();
    }
    else if (ans == 'n' || ans == 'N')
    {
     parameters = random_parameters ();
    }
    density = parameters[0];
    pInf = parameters[1];
    percInf = parameters[2];
    tMean = static_cast<int>(parameters[3]);
    range = static_cast<int>(parameters[4]);
    
    char ans2;
    bool quarantine = true;
    std::cout << "Quarantine option? (Y/N): ";
    std::cin >> ans2;
    while (ans2 != 'Y' && ans2 != 'y' && ans2 != 'N' && ans2 != 'n')
    {
        std::cout << "Invalid answer. Enter Y or N : ";
        std::cin >> ans2;
    }
    if (ans2 == 'y' || ans2 == 'Y')
    {
        quarantine = true;
    }
    if (ans2 == 'n' || ans2 == 'N')
    {
        quarantine = false;
    }
    
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

    population = population.epidemic(pInf, tMean, range, quarantine);
    // population = population.epidemic2(pInf,1./tMean);

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
  return 0;
}
