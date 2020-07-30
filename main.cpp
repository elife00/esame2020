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

  parameters = start(); // codice più pulito

  density = parameters[0];
  percInf = parameters[1];
  pInf = parameters[2];
  avgTime =
      static_cast<int>(parameters[3]); // in case of input of parameters it's
                                       // generated a vector of double
  range = static_cast<int>(
      parameters[4]); // it's better to force the convertion to integer
  bool quarantine = input_quarantine();

  std::chrono::microseconds iterationTime(250000);
  sf::Font font;
  // load font from file
  if (!font.loadFromFile(
          "../aBlackLives.ttf")) { // upload + controllo obbligatorio sfml
    throw std::runtime_error{"file not loaded"};
  };
  std::array<std::string, 7> string = {
      "Susceptible", "Infectious",   "Recovered",     "Quarantine",
      "Empty",       "<-        ->", "slower  faster"};
  std::array<sf::Color, 5> color = {Blue, Red, Yellow, sf::Color::White,
                                    sf::Color::Black};
  // cration of a legend of the colors used for differents states
  std::array<sf::Text, 7> legend = representLegend<5, 2>(font, string, color);

  int windowSize = sf::VideoMode::getDesktopMode().height * 3. / 4. +
                   50; // funzione che restituisce le dimensioni del dekstop -
                       // la window e la fa 3/4 + 50 pixel (proporzionata)
  sf::RenderWindow epidemicWindow(sf::VideoMode(windowSize, windowSize),
                                  "My epidemic"); // classe che crea le finestre
  sf::RenderWindow legendWindow(sf::VideoMode(200, 400), "Legend"); // pixel
  epidemicWindow.setVerticalSyncEnabled(true);

  // change the position of the window (relatively to the desktop)
  epidemicWindow.requestFocus(); // mette la window in primo piano
  epidemicWindow.setPosition(sf::Vector2i(
      (sf::VideoMode::getDesktopMode().width - epidemicWindow.getSize().x) / 2,
      (sf::VideoMode::getDesktopMode().height - epidemicWindow.getSize().x) /
          2)); // posizione centrata
  legendWindow.requestFocus();
  legendWindow.setPosition(sf::Vector2i(
      (sf::VideoMode::getDesktopMode().width + windowSize) / 2.,
      (sf::VideoMode::getDesktopMode().height - legendWindow.getSize().y) / 2));

  Board population(dim, density);
  Board::push_back({0,static_cast<int>(dim * dim * density), 0, 0});
  population.infection(percInf);
    
  // run the program as long as the window is open
  while (epidemicWindow.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event; // tipo di sfml - rileva gli eventi - interazione che
                     // stabilisci tu
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
    // create the graphic representation of the population
    representBoard rappresentation = population.draw();
    // crea un oggetto rappresentBoard passandogli il population_
    rappresentation.setPosition(epidemicWindow.getSize().x / 2,
                                epidemicWindow.getSize().y / 2);
    // draw the board of the population
    epidemicWindow.clear(sf::Color::Black); // sfondo
    epidemicWindow.draw(rappresentation);   // chiami l'oggetto da rappresentare
    epidemicWindow.display();

    // draw the legend
    legendWindow.clear(sf::Color::Black);
    for (int i = 0; i != 7; ++i)
      legendWindow.draw(legend[i]);

    legendWindow.display();

    // commands for the end of epidemy
    if (population.end()) {
      std::this_thread::sleep_for(std::chrono::seconds(3));
      epidemicWindow.close();
        legendWindow.close();
        population.trend();
        Situation final_s = population.situation();
        std::cout << '\n' << "Final Situation: " << '\n' << "Day: " << final_s.t << '\n' <<
        "Susceptibles: " << final_s.s << '\n' << "Infected: " << final_s.i << '\n' <<
        "Recovered: " << final_s.r << '\n';

      system("root");
    }

    // control of interation time
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      // left key is pressed: faster
      iterationTime -= std::chrono::microseconds(50000);
      // std::cout << iterationTime << '\n';
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      // left key is pressed: slower
      iterationTime += std::chrono::milliseconds(50);
      // std::cout << iterationTime << '\n';
    }
    // evolution of the epidemic
    auto [pop,sit] =  population.epidemic(pInf, avgTime, range, quarantine);
    population = pop;
    Board::push_back(sit);

    std::this_thread::sleep_for(iterationTime);
  }
  return 0;
}
