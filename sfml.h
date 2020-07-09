#ifndef sfml_h
#define sfml_h

#include "SFML/Graphics.hpp"
#include <cmath>

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
          sf::Color Suscetible(129, 156, 211, 255);
          quad[0].color = (Suscetible);
          quad[1].color = (Suscetible);
          quad[2].color = (Suscetible);
          quad[3].color = (Suscetible);
        } else if (vector[i + j * gridSize_] == I) {
          sf::Color Infected(211, 104, 107, 255);
          quad[0].color = (Infected);
          quad[1].color = (Infected);
          quad[2].color = (Infected);
          quad[3].color = (Infected);
        } else if (vector[i + j * gridSize_] == R) {
          sf::Color Recovered(91, 212, 86, 255);
          quad[0].color = (Recovered);
          quad[1].color = (Recovered);
          quad[2].color = (Recovered);
          quad[3].color = (Recovered);
        } else if (vector[i + j * gridSize_] == Q) {
          sf::Color Quarantine(221, 121, 39, 255);
          quad[0].color = (Quarantine);
          quad[1].color = (Quarantine);
          quad[2].color = (Quarantine);
          quad[3].color = (Quarantine);
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

#endif
