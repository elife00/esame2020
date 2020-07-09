#ifndef sfml_h
#define sfml_h

#include "SFML/Graphics.hpp"
#include <cmath>
#include <array>
#include <string>

enum class State : char {
  Empty,
  Susceptible,
  Infectious,
  Recovered,
  Quarantine
};
constexpr State S = State::Susceptible;
constexpr State I = State::Infectious;
constexpr State R = State::Recovered;
constexpr State Q = State::Quarantine;
constexpr State E = State::Empty;

class representBoard : public sf::Drawable,     // to draw
                       public sf::Transformable // to transform, to rotate, ...
{
private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // apply the transform
    states.transform *=
        getTransform(); // Transform combining the
                        // position/rotation/scale/origin of the object
    // draw the vertex array
    target.draw(vertices_, states);
  }
  int quadSize_;
  int gridSize_;
  sf::VertexArray vertices_;

public:
  representBoard(/*int const& quadSize,*/ std::vector<State> &vector)
      : /*quadSize_{quadSize}
      ,*/
        gridSize_{static_cast<int>(std::sqrt(vector.size()))} {
    vertices_.setPrimitiveType(sf::Quads); // reorganise vertices into quads
    vertices_.resize(gridSize_ * gridSize_ *
                     4); // create enough vertices for the grid
    quadSize_ = static_cast<int>(sf::VideoMode::getDesktopMode().height * 3. /
                                 4. / gridSize_);

    // populate the vertex array, with one quad per state
    for (int i = 0; i < gridSize_; ++i)
      for (int j = 0; j < gridSize_; ++j) {

        // get a pointer to the current state's quad
        sf::Vertex *quad = &vertices_[(i + j * gridSize_) * 4];

        // set the origin to the center of the grid
        sf::Transformable::setOrigin(gridSize_ * quadSize_ / 2,
                                     gridSize_ * quadSize_ / 2);

        // define its 4 corners
        quad[0].position = sf::Vector2f(i * quadSize_, j * quadSize_);
        quad[1].position = sf::Vector2f((i + 1) * quadSize_, j * quadSize_);
        quad[2].position =
            sf::Vector2f((i + 1) * quadSize_, (j + 1) * quadSize_);
        quad[3].position = sf::Vector2f(i * quadSize_, (j + 1) * quadSize_);

        if (vector[i + j * gridSize_] == S) {
          sf::Color Blu(129, 156, 255, 255);
          quad[0].color = (Blu);
          quad[1].color = (Blu);
          quad[2].color = (Blu);
          quad[3].color = (Blu);
        } else if (vector[i + j * gridSize_] == I) {
          sf::Color Red(240, 0, 0, 255);
          quad[0].color = (Red);
          quad[1].color = (Red);
          quad[2].color = (Red);
          quad[3].color = (Red);
        } else if (vector[i + j * gridSize_] == R) {
          sf::Color Green(91, 212, 20, 255);
          quad[0].color = (Green);
          quad[1].color = (Green);
          quad[2].color = (Green);
          quad[3].color = (Green);
        } else if (vector[i + j * gridSize_] == Q) {
          quad[0].color = (sf::Color::White);
          quad[1].color = (sf::Color::White);
          quad[2].color = (sf::Color::White);
          quad[3].color = (sf::Color::White);
        } else if (vector[i + j * gridSize_] == E) {
          quad[0].color = (sf::Color::Black);
          quad[1].color = (sf::Color::Black);
          quad[2].color = (sf::Color::Black);
          quad[3].color = (sf::Color::Black);
        }
      }
  }

  sf::VertexArray vertices() { return vertices_; }
};

inline std::array<sf::Text, 5> legend() {
  std::array<sf::Text, 5> legend_;
  sf::Font font;
  font.loadFromFile("aBlackLives.ttf");
  std::array<std::string, 5> string = {"Susceptible", "Infectious", "Recovered",
                                  "Quarantine", "Empty"};
  std::array<sf::Color, 5> color = {
      sf::Color(129, 156, 255, 255), sf::Color(240, 0, 0, 255),
      sf::Color(91, 212, 20, 255), sf::Color::White, sf::Color::Black};
  for (int i = 0; i != 5; ++i) {
    legend_[i].setFont(font);
    legend_[i].setString(string[i]);
    legend_[i].setCharacterSize(24);
    legend_[i].setFillColor(color[i]);
    legend_[i].setPosition(0, i * 24);
  }
  return legend_;
}

#endif
