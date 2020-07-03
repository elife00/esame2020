#ifndef sfml_h
#define sfml_h

#include <cmath>
#include "SFML/Graphics.hpp"

enum class State : char { Susceptible, Infectious, Recovered, Empty };
constexpr State S = State::Susceptible;
constexpr State I = State::Infectious;
constexpr State R = State::Recovered;
constexpr State E = State::Empty;

class representBoard
    : public sf::Drawable,      // per disegnarlo
      public sf::Transformable  // per traslazioni, rotazioni, ...
{
 private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
  {
    // apply the transform
    states.transform *=
        getTransform();  // Transform combining the
                         // position/rotation/scale/origin of the object
    // draw the vertex array
    target.draw(vertices_, states);
  }
  int quadSize_;
  int gridSize_;
  sf::VertexArray vertices_;

 public:
  representBoard(int const& quadSize, std::vector<State>& vector)
      : quadSize_{quadSize}
      , gridSize_{static_cast<int>(std::sqrt(vector.size()))}
  {
    vertices_.setPrimitiveType(sf::Quads);  // riorganizza i vertici in quadrati
    vertices_.resize(gridSize_ * gridSize_ *
                     4);  // crea abbastanza vertici per la griglia

    // populate the vertex array, with one quad per tile
    for (int i = 0; i < gridSize_; ++i)
      for (int j = 0; j < gridSize_; ++j) {
        // get the current tile number

        // get a pointer to the current tile's quad
        sf::Vertex* quad = &vertices_[(i + j * gridSize_) * 4];

        sf::Transformable::setOrigin(gridSize_ * quadSize_ / 2,
                                     gridSize_ * quadSize_ / 2);

        // define its 4 corners
        quad[0].position = sf::Vector2f(i * quadSize, j * quadSize);
        quad[1].position = sf::Vector2f((i + 1) * quadSize, j * quadSize);
        quad[2].position = sf::Vector2f((i + 1) * quadSize, (j + 1) * quadSize);
        quad[3].position = sf::Vector2f(i * quadSize, (j + 1) * quadSize);

        if (vector[i + j * gridSize_] == S) {
          quad[0].color = (sf::Color::White);
          quad[1].color = (sf::Color::White);
          quad[2].color = (sf::Color::White);
          quad[3].color = (sf::Color::White);
        } else if (vector[i + j * gridSize_] == I) {
          quad[0].color = (sf::Color::Red);
          quad[1].color = (sf::Color::Red);
          quad[2].color = (sf::Color::Red);
          quad[3].color = (sf::Color::Red);
        } else if (vector[i + j * gridSize_] == R) {
          quad[0].color = (sf::Color::Green);
          quad[1].color = (sf::Color::Green);
          quad[2].color = (sf::Color::Green);
          quad[3].color = (sf::Color::Green);
        } else if (vector[i + j * gridSize_] == E) {
          quad[0].color = (sf::Color::Black);
          quad[1].color = (sf::Color::Black);
          quad[2].color = (sf::Color::Black);
          quad[3].color = (sf::Color::Black);
        }
      }
  }
};

#endif
