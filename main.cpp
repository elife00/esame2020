#include "epidemic.hpp"
#include "functions.hpp"
#include "sfml.hpp"

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include <thread>

constexpr int dim = 120;

int main() {

  double density;
  double pInf;
  double percInf;
  int avgTime;
  int range;
  std::array<double, 5> parameters;
  int iterationTime = 250;
  // cration of a legend of the colors used for differents states
  std::array<sf::Text, 7> legend;
  sf::Font font;
  // load font from file
  if (!font.loadFromFile("../aBlackLives.ttf")) {
    throw std::runtime_error{"file not loaded"};
  };
  std::array<std::string, 7> string = {
      "Susceptible", "Infectious",   "Recovered",     "Quarantine",
      "Empty",       "<-        ->", "slower  faster"};
  std::array<sf::Color, 5> color = {Blue, Red, Yellow, sf::Color::White,
                                    sf::Color::Black};
  for (int i = 0; i != 5; ++i) {
    legend[i].setFont(font);
    legend[i].setString(string[i]);
    legend[i].setCharacterSize(22);
    legend[i].setFillColor(color[i]);
    legend[i].setPosition(100 - legend[i].getLocalBounds().width / 2,
                          (i + 1) * 40);
  }
  legend[4].setOutlineColor(sf::Color::White);
  legend[4].setOutlineThickness(2);

  for (int i = 5; i != 7; ++i) {
    legend[i].setFont(font);
    legend[i].setString(string[i]);
    legend[i].setCharacterSize(17);
    legend[i].setFillColor(color[3]);
    legend[i].setPosition(100 - legend[i].getLocalBounds().width / 2,
                          (i + 2) * 40);
  }

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
  avgTime =
      static_cast<int>(parameters[3]); // in case of input of parameters it's
                                       // generated a vector of double
  range = static_cast<int>(
      parameters[4]); // it's better to force the convertion to integer
  bool quarantine = input_quarantine();

  int windowSize = sf::VideoMode::getDesktopMode().height * 3. / 4. + 50;
  sf::RenderWindow epidemicWindow(sf::VideoMode(windowSize, windowSize),
                                  "My epidemic");
  sf::RenderWindow legendWindow(sf::VideoMode(200, 400), "Legend");
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
    // crate the graphic representation of the population
    representBoard rappresentation = population.draw();
    rappresentation.setPosition(epidemicWindow.getSize().x / 2,
                                epidemicWindow.getSize().y / 2);
    // draw the board of the population
    epidemicWindow.clear(sf::Color::Black);
    epidemicWindow.draw(rappresentation);
    epidemicWindow.display();

    // draw the legend
    legendWindow.clear(sf::Color::Black);
    for (int i = 0; i != 7; ++i)
      legendWindow.draw(legend[i]);
    legendWindow.display();

    // commands for the end of epidemy
    if (population.end() == 0) {
      std::this_thread::sleep_for(std::chrono::seconds(3));
      epidemicWindow.close();
      legendWindow.close();
      population.trend();

      system("root");
    }

    // control of interation time
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
    // evolution of the epidemic
    population = population.epidemic(pInf, avgTime, range, quarantine);

    std::this_thread::sleep_for(std::chrono::milliseconds(iterationTime));
  }
  return 0;
}
