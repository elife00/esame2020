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

struct Situation {
  int t = 0;
  int s = 0;
  int i = 0;
  int r = 0;
};

inline bool operator==(Situation const &s1, Situation const &s2) {
  return (s1.t == s2.t && s1.s == s2.s && s1.i == s2.i && s1.r == s2.r);
}

class Board {
private:
  int n_;
  double density_;
  std::vector<State> board_;
  std::vector<int> stay_;
  std::vector<Situation> evolution_;

public:
  Board(int n, double d)
      : n_{n}, density_{d}, board_(n * n),
        stay_(n * n), evolution_{{0, static_cast<int>(n * n * d), 0, 0}} {
    assert(density_ > 0 && density_ <= 1.);
    int people = static_cast<int>(n * n * density_);
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

  int contact_range(int x, int y, int r) const;

  Situation current_situation();

  void set(int &x, int &y, State s);

  void swap(int x, int y);

  Board epidemic(double pInf, int tMean, int range, bool quarantine);

  // Board epidemic2(double pInf, double pGua);

  representBoard Draw(/*int const& quadSize*/);

  double avg_time() const;

  void trend() const;

  void parameters();
};

#endif /* epidemic_hpp */
