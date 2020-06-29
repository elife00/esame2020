#include "epidemic3.hpp"

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include <thread>

// g++ -std=c++17 gui.cpp -o gui -lsfml-graphics -lsfml-window -lsfml-system

int main() {

  sf::RenderWindow epidemicWindow(sf::VideoMode(800, 600), "My epidemic");
  epidemicWindow.setVerticalSyncEnabled(true);

  // change the position of the window (relatively to the desktop)
  epidemicWindow.setPosition(sf::Vector2i(10, 50));

  int i = 1;
  
  int dim = 100;
  double pInf = 0.5;
  double pGua = 0.1;
  double ratInf = 0.2;
  int tMean = 10;
  int quadSize = 10;
  double density = 0.5;

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
    rappresentation.setPosition(epidemicWindow.getSize().x/2, epidemicWindow.getSize().y/2);
    
    epidemicWindow.clear(sf::Color::Black);
    epidemicWindow.draw(rappresentation);
    epidemicWindow.display();
    
    
    if (population.count()) {
      if (i) {
        population.trend();
        system("root");
        --i;
      }
    } else {
      population = population.epidemic(pInf, tMean);
    }
    
       
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    
  }
  return 0;
}
