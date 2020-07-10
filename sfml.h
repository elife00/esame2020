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


inline sf::Color Blue(0, 0, 255, 255);
inline sf::Color Red(255, 0, 0, 255);
inline sf::Color Green(0, 255, 0, 255);

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
  representBoard(std::vector<State> &vector)
      : gridSize_{static_cast<int>(std::sqrt(vector.size()))} {
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
        quad[2].position = sf::Vector2f((i + 1) * quadSize_, (j + 1) * quadSize_);
        quad[3].position = sf::Vector2f(i * quadSize_, (j + 1) * quadSize_);

        if (vector[i + j * gridSize_] == S) {
          quad[0].color = (Blue);
          quad[1].color = (Blue);
          quad[2].color = (Blue);
          quad[3].color = (Blue);
        } else if (vector[i + j * gridSize_] == I) {
          quad[0].color = (Red);
          quad[1].color = (Red);
          quad[2].color = (Red);
          quad[3].color = (Red);
        } else if (vector[i + j * gridSize_] == R) {
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

inline auto legend (sf::Font font, std::array<std::string, 5> labels, std::array<sf::Color, 5> labelsColor) {
  
  std::array<sf::Text, 5> legend_;
      
    for (int i = 0; i != 5; ++i) {
      legend_[i].setFont(font);
      legend_[i].setString(labels[i]);
      legend_[i].setCharacterSize(22);
      legend_[i].setFillColor(labelsColor[i]);
      legend_[i].setPosition(100 - legend_[i].getLocalBounds().width / 2, (i + 1) * 40);
    }
    legend_[4].setOutlineColor(sf::Color::White);
    legend_[4].setOutlineThickness(3);
    
  return legend_;
}

#endif
