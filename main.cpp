#include "epidemic.hpp"

#include <thread>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

// g++ -std=c++17 gui.cpp -o gui -lsfml-graphics -lsfml-window -lsfml-system
// g++ -std=c++17 functions.test.cpp -o gui
// mkdir build
//cd build
//cmake -DCMAKE_BUILD_TYPE=Debug ..
//make
//./epidemic-sfml
// dopo la prima volta bastano gli ultimi due

int main()
{
  sf::RenderWindow epidemicWindow(
      sf::VideoMode(sf::VideoMode::getDesktopMode().height * 2 / 3,
                    sf::VideoMode::getDesktopMode().height * 2 / 3),
      "My epidemic");
  epidemicWindow.setVerticalSyncEnabled(true);

  // change the position of the window (relatively to the desktop)
  epidemicWindow.requestFocus();
  epidemicWindow.setPosition(sf::Vector2i(
      (sf::VideoMode::getDesktopMode().width - epidemicWindow.getSize().x) / 2,
      (sf::VideoMode::getDesktopMode().height - epidemicWindow.getSize().x) /
          2));

  int i = 1;

  int dim = 100;
  double pInf = 0.25;
  double pGua = 0.15;
  double ratInf = 0.1;
  int tMean = 10;
  int quadSize = 5;
  double density = 0.20;

  Board population(dim, density);
  population.infection(ratInf);

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

    if (population.count()) {
      // if (i) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      epidemicWindow.close();
      population.trend();

      system("root");
      //--i;
    }
    //} else {
    population = population.epidemic2(pInf, 1. / tMean);
    // population = population.epidemic(pInf,tMean);
    //}

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  return 0;
}
