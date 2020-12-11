#include "epidemic.hpp"
#include "functions.hpp"
#include "sfml.hpp"

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include <iostream>
#include <thread>

constexpr int dim = 300;

int main() {

  double density;
  double pInf;
  double percInf;
  int avgTime;
  int range;
  std::array<double, 5> parameters;

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
  std::string string[11] = {"Susceptible",    "Infectious",    "Recovered",
                            "Quarantine",     "Empty",         "<-        ->",
                            "slower  faster", "Susceptible: ", "Infectious: ",
                            "Recovered: ",    "Elapsed days: "};

  std::array<sf::Color, 5> color = {Blue, Red, Yellow, sf::Color::White,
                                    sf::Color::Black};
  // cration of a legend of the colors used for differents states
  std::array<sf::Text, 11> legend = representLegend<5, 6>(font, string, color);

  sf::Texture texture;
  if (!texture.loadFromFile(
          "ModelloSir.gif")) { // upload + controllo obbligatorio sfml
    throw std::runtime_error{"file not loaded"};
  };

  int windowSize = sf::VideoMode::getDesktopMode().height * 3. / 4. +
                   50; // funzione che restituisce le dimensioni del dekstop -
                       // la window e la fa 3/4 + 50 pixel (proporzionata)
  sf::RenderWindow epidemicWindow(sf::VideoMode(windowSize, windowSize),
                                  "My epidemic"); // classe che crea le finestre
  sf::RenderWindow legendWindow(sf::VideoMode(220, 600), "Legend"); // pixel
  sf::RenderWindow graphWindow(
      sf::VideoMode(texture.getSize().x, texture.getSize().y), "Graph");
      epidemicWindow.setVerticalSyncEnabled(true);

  // change the position of the window (relatively to the desktop)
  epidemicWindow.setPosition(sf::Vector2i(
      (sf::VideoMode::getDesktopMode().width - epidemicWindow.getSize().x) / 2,
      (sf::VideoMode::getDesktopMode().height - epidemicWindow.getSize().x) /
          2)); // posizione centrata
  legendWindow.requestFocus();
  epidemicWindow.requestFocus(); // mette la window in primo piano
  legendWindow.setPosition(sf::Vector2i(
      (sf::VideoMode::getDesktopMode().width + windowSize) / 2.,
      (sf::VideoMode::getDesktopMode().height - legendWindow.getSize().y) / 2));

  Board population(dim, density);
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
        graphWindow.close();
      }
    }

    while (legendWindow.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        legendWindow.close();
      }
    }

    while (graphWindow.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        graphWindow.close();
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

    Situation situation = population.situation();
    legend[7].setString("Susceptible: " + std::to_string(situation.s));
    legend[8].setString("Infectious: " + std::to_string(situation.i));
    legend[9].setString("Recovered: " + std::to_string(situation.r));
    legend[10].setString("Elapsed days: " + std::to_string(situation.t));
    for (int i = 7; i != 11; ++i)
      legend[i].setPosition(100 - legend[i].getLocalBounds().width / 2,
                            (i + 2) * 40);

    // draw the legend
    legendWindow.clear(sf::Color::Black);
    for (int i = 0; i != 11; ++i)
      legendWindow.draw(legend[i]);

    legendWindow.display();

    population.trend();

    system("g++ ../graph.cpp `root-config --cflags --libs`");
    system("./a.out");
    if (!texture.loadFromFile(
            "ModelloSir.gif")) { // upload + controllo obbligatorio sfml
      throw std::runtime_error{"file not loaded"};
    };
    sf::Sprite graph(texture);
    graph.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
    graph.setPosition(graphWindow.getSize().x / 2, graphWindow.getSize().y / 2);
    graphWindow.clear(sf::Color::Black); // sfondo
    graphWindow.draw(graph);             // chiami l'oggetto da rappresentare
    graphWindow.display();

    // control of interation time
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      // left key is pressed: faster
      iterationTime -= std::chrono::microseconds(50000);
      if (iterationTime == std::chrono::milliseconds(0))
        std::cout << "This is the maximum speed of epidemic's evolution!\n";
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      // left key is pressed: slower
      iterationTime += std::chrono::milliseconds(50);
    }

    // evolution of the epidemic
    if (!population.end()) {
      population = population.epidemic(pInf, avgTime, range, quarantine);
      std::this_thread::sleep_for(iterationTime);
    }
  }
  return 0;
}
