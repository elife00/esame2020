#include "epidemic.hpp"

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include <thread>
#include <iostream>

// g++ -std=c++17 gui.cpp -o gui -lsfml-graphics -lsfml-window -lsfml-system

int main() {

  sf::RenderWindow epidemicWindow(sf::VideoMode(800, 600), "My epidemic");
  //sf::RenderWindow graphWindow(sf::VideoMode(600, 600), "My graph");
  epidemicWindow.setVerticalSyncEnabled(true);

  // change the position of the window (relatively to the desktop)
  epidemicWindow.setPosition(sf::Vector2i(10, 50));
  //graphWindow.setPosition(sf::Vector2i(850, 50));

  int i = 1;

<<<<<<< HEAD
  
  int dim = 200;
  double pInf = 0.15;
  double pGua = 0.1;
=======
  int dim = 200;
  double pInf = 0.05;
  double pGua = 0.09;
>>>>>>> 9d8cf1195b0bd2335171b3036755dee0417a17b6
  double ratInf = 0.2;
  int tMean = 30;
  int quadSize = 10;

  Population population(dim);
  population.infection(ratInf);

  

  // run the program as long as the window is open
  while (epidemicWindow.isOpen() || graphWindow.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (epidemicWindow.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        epidemicWindow.close();
        graphWindow.close();
      }
    }
    while (graphWindow.pollEvent(event)) {
      if (event.type == sf::Event::EventType::Closed) {
        graphWindow.close();
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
       
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    
  }
<<<<<<< HEAD
  /*population.trend();
  system("root");*/
=======
    
  population.trend();
  system("root");
>>>>>>> 9d8cf1195b0bd2335171b3036755dee0417a17b6

  return 0;
}
