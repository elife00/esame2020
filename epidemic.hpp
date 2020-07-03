#ifndef epidemic_hpp
#define epidemic_hpp

#include "sfml.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

struct Situation
{
  int t = 0;
  int s = 0;
  int i = 0;
  int r = 0;
};

class Board
{
 private:
  int n_;
  double density_;
  std::vector<State> board_;
  std::vector<int> stay_;
  std::vector<Situation> evolution_;

 public:
  Board(int n, double d)
      : n_{n}
      , density_{d}
      , board_(n * n)
      , stay_(n * n)
      , evolution_{{0, (int)(n * n * d), 0, 0}}
  {
    assert(density_ > 0 && density_ < 1);
    int people = (int)(n * n * density_);
    for (int i = 0; i < people; ++i) {
      board_[i] = S;
    }
    for (int i = people; i < n_ * n_; ++i) {
      board_[i] = E;
    }
    std::random_device seed;
    std::mt19937 g(seed());
    std::shuffle(board_.begin(), board_.end(), g);
  }

  State get(int x, int y) const;

  void infection(double ratInf);

  int contact(int x, int y) const;

  void set(int& x, int& y, State s);

  void swap(int x, int y);

  Board epidemic(double pInf, int tMean);

  Board epidemic2(double pInf, double pGua);

  representBoard Draw(int const& quadSize);

  double tMean() const;

  void trend() const;

  bool count() const;

  void parameters();
};

#endif /* epidemic_hpp */
