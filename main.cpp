#include "epidemic.hpp"
#include "functions.h"
#include "sfml.h"

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

int main() {

  double density;
  double pInf;
  double percInf;
  int avgTime;
  int range;
  std::array<double, 5> parameters;
  int iterationTime = 250;
  std::array<sf::Text, 5> legend_;
  sf::Font font;
  if (!font.loadFromFile("./aBlackLives.ttf")) {
    throw std::runtime_error{"file not loaded"};
  };
  std::array<std::string, 5> string = {"Susceptible", "Infectious", "Recovered",
                                       "Quarantine", "Empty"};
  std::array<sf::Color, 5> color = {Blue, Red, Green, sf::Color::White,
                                    sf::Color::Black};
  for (int i = 0; i != 5; ++i) {
    legend_[i].setFont(font);
    legend_[i].setString(string[i]);
    legend_[i].setCharacterSize(22);
    legend_[i].setFillColor(color[i]);
    legend_[i].setPosition(100 - legend_[i].getLocalBounds().width / 2,
                           (i + 1) * 40);
  }
  legend_[4].setOutlineColor(sf::Color::White);
  legend_[4].setOutlineThickness(2);

  std::cout << "Do you want to enter the epidemic's parameters? (Otherwise "
               "will be produced a random epidemic). (Y/N): "
            << '\n';
  char ans;
  std::cin >> ans;
  while (ans != 'Y' && ans != 'y' && ans != 'N' && ans != 'n') {
    std::cout << "Invalid answer. Enter Y or N : ";
    std::cin >> ans;
  }
  if (ans == 'y' || ans == 'Y') {
    parameters = input_parameters();
  } else if (ans == 'n' || ans == 'N') {
    parameters = random_parameters();
  }
  density = parameters[0];
  pInf = parameters[1];
  percInf = parameters[2];
  avgTime = static_cast<int>(
      parameters[3]); // nel caso di input si ha un vettore di double
  range = static_cast<int>(
      parameters[4]); // quindi è consigliata la forzatura ad intero
  bool quarantine = input_quarantine();

  int windowSize = sf::VideoMode::getDesktopMode().height * 3. / 4. + 50;
  sf::RenderWindow epidemicWindow(sf::VideoMode(windowSize, windowSize),
                                  "My epidemic");
  sf::RenderWindow legendWindow(sf::VideoMode(200, 400), "My epidemic");
  epidemicWindow.setVerticalSyncEnabled(true);

  // change the position of the window (relatively to the desktop)
  epidemicWindow.requestFocus();
  epidemicWindow.setPosition(sf::Vector2i(
      (sf::VideoMode::getDesktopMode().width - epidemicWindow.getSize().x) / 2,
      (sf::VideoMode::getDesktopMode().height - epidemicWindow.getSize().x) /
          2));
  legendWindow.requestFocus();
  legendWindow.setPosition(sf::Vector2i(
      (sf::VideoMode::getDesktopMode().width + windowSize) / 2.,
      (sf::VideoMode::getDesktopMode().height - legendWindow.getSize().y) / 2));

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
        legendWindow.close();
      }
    }

    while (legendWindow.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        legendWindow.close();
      }
    }

    auto rappresentation = population.draw();
    rappresentation.setPosition(epidemicWindow.getSize().x / 2,
                                epidemicWindow.getSize().y / 2);

    epidemicWindow.clear(sf::Color::Black);
    epidemicWindow.draw(rappresentation);
    epidemicWindow.display();

    legendWindow.clear(sf::Color::Black);

    legendWindow.draw(legend_[0]);
    legendWindow.draw(legend_[1]);
    legendWindow.draw(legend_[2]);
    legendWindow.draw(legend_[3]);
    legendWindow.draw(legend_[4]);
    legendWindow.display();

    if (population.current_situation().i == 0) {
      std::this_thread::sleep_for(std::chrono::seconds(3));
      epidemicWindow.close();
      legendWindow.close();
      population.trend();

      system("root");
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      // left key is pressed: faster
      iterationTime -= 50;
      std::cout << iterationTime << '\n';
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      // left key is pressed: slower
      iterationTime += 50;
      std::cout << iterationTime << '\n';
    }

    population = population.epidemic(pInf, avgTime, range, quarantine);

    std::this_thread::sleep_for(std::chrono::milliseconds(iterationTime));
  }
  return 0;
}
