#ifndef epidemic_hpp
#define epidemic_hpp

#include "sfml.hpp"

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
  int t = 0; // iteration's index
  int s = 0; // n susceptible people
  int i = 0; // n infected people
  int r = 0; // n recovered people
};

// necessary for the tests
inline bool operator==(Situation const &s1, Situation const &s2) {
  return (s1.t == s2.t && s1.s == s2.s && s1.i == s2.i && s1.r == s2.r);
}

class Board {
private:
  int n_;
  double density_;
  std::vector<State> board_;
  std::vector<int> stay_; // index of the number of days that the i-th cell
                          // passed in the state Infected
  //std::vector<Situation> evolution_; // saving the situation at every iteration
  static std::vector<Situation> evolution_;

public: // the constructor sets all alive cells (people) to susceptible and than
        // shuffles
  Board(int n, double d)
      : n_{n}, density_{d}, board_(n * n),
        stay_(n * n)/*, evolution_{
                          {0, static_cast<int>(n * n * density_), 0, 0}}*/ {
    assert(density_ > 0 && density_ <= 1.);
    int people = static_cast<int>(n * n * d);

    //std::fill(board_.begin(), board_.begin() + people, S);
    //std::fill(board_.begin() + people, board_.end(), E);    
    
    for (int i = 0; i < people; ++i) {
      board_[i] = S;
    }
    for (int i = people; i < n_ * n_; ++i) {
      board_[i] = E;
    }
    
    std::random_device seed;
    std::mt19937 g(seed());
    std::shuffle(board_.begin(), board_.end(), g);

    evolution_.push_back({0, static_cast<int>(n * n * density_), 0, 0});
  }


  State get(int x, int y) const;

  void infection(double ratInf);

  int contact(int x, int y, int r) const;

  void set(int x, int y, State s);

  void swap(int x, int y);

  Board epidemic(double pInf, int avgTime, int range, bool quarantine);

  representBoard draw();

  double avg_time();

  void trend();

  Situation situation() const;

  bool end();
};

#endif /* epidemic_hpp */
