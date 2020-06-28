#include "epidemic.hpp"

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include <thread>

// g++ -std=c++17 gui.cpp -o gui -lsfml-graphics -lsfml-window -lsfml-system

int main() {

  sf::RenderWindow epidemicWindow(sf::VideoMode(800, 600), "My epidemic");
  sf::RenderWindow graphWindow(sf::VideoMode(600, 600), "My graph");
  epidemicWindow.setVerticalSyncEnabled(true);

  // change the position of the window (relatively to the desktop)
  epidemicWindow.setPosition(sf::Vector2i(10, 50));
  graphWindow.setPosition(sf::Vector2i(850, 50));

  // immagine su finestra graphWindow
  sf::Texture texture;
  if (!texture.loadFromFile("tileset.png")) {
    return EXIT_FAILURE;
  }
  texture.setSmooth(true);
  sf::Sprite graphic(texture);

  // set position of graphic in the window graph
  graphic.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
  graphic.setPosition(
      sf::Vector2f(graphWindow.getSize().x / 2,
                   graphWindow.getSize().y / 2)); // absolute position

  int dim = 200;
  double pInf = 0.05;
  double pGua = 0.09;
  double ratInf = 0.2;
  int tMean = 10;
  int quadSize = 10;

  Population population(dim);
  population.infection(ratInf);

  sf::Clock clock;

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

    graphWindow.clear(sf::Color::Black);
    graphWindow.draw(graphic);
    graphWindow.display();

    auto rappresentation = population.Draw(quadSize);
    rappresentation.setPosition(epidemicWindow.getSize().x/2, epidemicWindow.getSize().y/2);

    epidemicWindow.clear(sf::Color::Black);
    epidemicWindow.draw(rappresentation);
    epidemicWindow.display();

    
    population = population.epidemic(pInf, tMean);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    
  }
    
  population.trend();
  system("root");

  return 0;
}
