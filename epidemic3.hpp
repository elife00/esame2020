#ifndef EPIDEMIC_HPP
#define EPIDEMIC_HPP

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "SFML/Graphics.hpp"

enum class State : char { Susceptible, Infectious, Recovered, Empty };
constexpr State S = State::Susceptible;
constexpr State I = State::Infectious;
constexpr State R = State::Recovered;
constexpr State E = State::Empty;

struct Situation
{
  int t = 0;
  int s = 0;
  int i = 0;
  int r = 0;
};

long double factorial(int n)
{
  if (n == 0) {
    return 1;
  }
  long double factorial = 1;
  for (int i = 1; i <= n; ++i) {
    factorial *= i;
  }
  assert(factorial > 0);
  return factorial;
}

long double cumulative(int k, int mu)
{
  assert(k > 0);
  long double p = 0;
  for (int i = 0; i <= k; ++i) {
    p += (pow(mu, i) * exp(-mu) / factorial(i));
  }
  return p;
}

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
      , board_(n*n)
      , stay_(n * n)
      , evolution_{{0, (int)(n * n * d), 0, 0}}
  {
    assert(density_ > 0 && density_ < 1);
    int people = (int)(n * n * density_);
    for (int i = 0; i < people; ++i) {
      board_[i] = S;
    }
    for (int i = people; i < n_*n_; ++i) {
      board_[i] = E;
    }
    std::random_device seed;
    std::mt19937 g(seed());
    std::shuffle(board_.begin(), board_.end(), g);
  }

    
    State get (int x, int y) const
    {
      return (x < 1 || x > n_ || y < 1 || y > n_) 
      ? E
      : board_[(y - 1) * n_ + (x - 1)];
    }

void infection(double ratInf)
{
  int infected = (int)(ratInf * density_ * n_ * n_);
  int j = 0;
  for (int i = 0; i != infected; ++j) {
    if (board_[j] == E) {
      ++j;
    } else {
    board_[j] = I;
    ++i;
    }
  }
  std::random_device seed;
  std::mt19937 g(seed());
  std::shuffle(board_.begin(), board_.end(), g);
}

int contact(int x, int y) const
{
  assert(x > 0 && x <= n_ && y > 0 && y <= n_);
  int count = 0;
  if (get(x + 1, y) == I) {
    ++count;
  }
  if (get(x - 1, y) == I) {
    ++count;
  }
  if (get(x, y + 1) == I) {
    ++count;
  }
  if (get(x, y - 1) == I) {
    ++count;
  }
  if (get(x - 1, y - 1) == I) {
    ++count;
  }
  if (get(x - 1, y + 1) == I) {
    ++count;
  }
  if (get(x + 1, y - 1) == I) {
    ++count;
  }
  if (get(x + 1, y + 1) == I) {
    ++count;
  }
  return count;
}

void set(int& x, int& y, State s)
{
  assert(x > 0 && x <= n_ && y > 0 && y <= n_);
  board_[(y - 1) * n_ + (x - 1)] = s;
}

void swap(int x, int y)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(-1, 1);

  int i = dis(gen);
  int j = dis(gen);
  int x2 = x + i;
  int y2 = y + j;
  if (((i != 0) || (j != 0)) && x2 > 0 && x2 <= n_ && y2 > 0 && y2 <= n_ && get(x2, y2) == E) {
      set(x2, y2, get(x, y));
      set(x, y, E);
      stay_[(y2 - 1) * n_ + (x2 - 1)] = stay_[(y - 1) * n_ + (x - 1)];
      stay_[(y - 1) * n_ + (x - 1)] = 0;
  }
}

Board epidemic(double pInf, int tMean)
{
  assert(pInf > 0 && pInf < 1 && tMean > 0 && tMean < 40);
  Board next(n_, density_);
  next.evolution_ = evolution_;
  next.stay_ = stay_;
  Situation sit = {0, 0, 0, 0};

  std::random_device rd;
  std::mt19937 gen(rd());

  for (int x = 1; x != n_ + 1; ++x) {
    for (int y = 1; y != n_ + 1; ++y) {
      int coordinate = (y - 1) * n_ + (x - 1);
      
      if (board_[coordinate] == S) {
        ++sit.s;
        int infected = contact(x, y);

        std::binomial_distribution<> dis(infected, pInf);

        if (dis(gen) > 0) {
          next.set(x, y, I);
        } else {
          next.set(x, y, S);
        }
      }
      if (board_[coordinate] == I) {
        ++sit.i;
        int iDays = ++next.stay_[coordinate];

        std::uniform_real_distribution<> dis(0., 1.0);

        double pGua = cumulative(iDays, tMean);

        if (dis(gen) <= pGua) {
          next.set(x, y, R);
        } else {
          next.set(x, y, I);
        }
      }
      if (board_[coordinate] == R) {
        ++sit.r;
        next.set(x, y, R);
      }
      if (board_[coordinate] == E) {
        next.set(x, y, E);
      }
    }
  }
  for (int x = 1; x != n_ + 1; ++x) {
    for (int y = 1; y != n_ + 1; ++y) {
      int coordinate = (y - 1) * n_ + (x - 1);
      if (board_[coordinate] != E) 
      {
        next.swap(x, y);
      }
    }
  }

  sit.t = ++evolution_.back().t;
  next.evolution_.push_back({sit});

  return next;
}

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
    vertices_.resize(gridSize_ * gridSize_ * 4);  // crea abbastanza vertici per la griglia

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

representBoard Draw(int const& quadSize)
{
  Board::representBoard rappresentation(quadSize, board_);
  return rappresentation;
}

void draw_stay() const
{
  for (int y = n_; y > 0; --y) {
    for (int x = 1; x <= n_; ++x) {
      std::cout << stay_[(y - 1) + (x - 1) * n_];
    }
    std::cout << '\n';
  }
}

double tMean() const
{
  double i = 0.;
  int a = std::accumulate(stay_.begin(), stay_.end(), 0);
  for (auto v : stay_) {
    if (v != 0) {
      ++i;
    }
  }
  return a / i;
}

void trend() const
{
  std::ofstream fout;
  fout.open("trend.txt");
  for (auto& v : evolution_) {
    fout << std::setw(4) << v.t << ' ' << std::setw(4) << v.s << ' '
         << std::setw(4) << v.i << ' ' << std::setw(4) << v.r << ' '
         << std::setw(4) << tMean() <<   std::setw(4) << ' ' << v.i+v.r+v.s << '\n';
  }

  fout.close();
}

bool count() const
{
  int i = 0;
  int s = 0;
  int r = 0;
  for (auto const& v : board_) {
    if (v == S) {
      ++s;
    } else if (v == I) {
      ++i;
    } else if (v == R) {
      ++r;
    }
  }
  if (i == 0) {
    return true;
  }
 
  return false;
}

void parameters()
{
  std::vector<double> beta;
  std::vector<double> gamma;
  for (int i = 0; i != evolution_.size() - 1; ++i) {  // le formule sono con le percentuali
    double Savg = 0.5 * (evolution_[i + 1].s + evolution_[i].s) / n_ * n_;
    double Iavg = 0.5 * (evolution_[i + 1].i + evolution_[i].i) / n_ * n_;
    double Ravg = 0.5 * (evolution_[i + 1].r + evolution_[i].r) / n_ * n_;
    double dS = (evolution_[i + 1].s - evolution_[i].s) / n_ * n_;
    double dI = (evolution_[i + 1].i - evolution_[i].i) / n_ * n_;
    double dR = (evolution_[i + 1].r - evolution_[i].r) / n_ * n_;
    double b = (dR + dI) / (Savg * Iavg);
    double g = dR / Iavg;
    beta.push_back(b);
    gamma.push_back(g);
  }
  for (int i = 0; i != beta.size(); ++i) {
    std::cout << beta[i] << ' ' << gamma[i] << '\n';
  }
}

};

#endif
