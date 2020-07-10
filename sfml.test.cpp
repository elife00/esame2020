#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "sfml.hpp"
#include <random>

TEST_CASE("testing sfml") {
  std::vector<State> vector(25, E);
  representBoard board(vector);
  sf::VertexArray vertices = board.vertices();
  unsigned verticesSize = 4 * vector.size();
  // check if the number of vertex generatated are enough for the representation
  // of the vector
  CHECK(vertices.getVertexCount() == verticesSize);
  // check black color for empty
  for (unsigned i = 0; i < verticesSize; ++i) {
    CHECK(vertices[i].color == sf::Color::Black);
  }

  for (int i = 0; i < 15; ++i) {
    vector[i] = I;
  }
  std::random_device g;
  std::shuffle(vector.begin(), vector.end(), g);
  board = representBoard(vector);
  vertices = board.vertices();
  int infected = 0;
  int empty = 0;
  // check the number of rad vertex and black vertex
  for (unsigned i = 0; i < vertices.getVertexCount(); ++i) {
    if (vertices[i].color == sf::Color::Red) {
      ++infected;
    }
    if (vertices[i].color == sf::Color::Black) {
      ++empty;
    }
  }
  CHECK(infected == (15 * 4));
  CHECK(empty == ((25 - 15) * 4));
}
