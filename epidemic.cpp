#include "epidemic.hpp"
#include "functions.h"

State Board::get(int x, int y) const
{
  return (x < 1 || x > n_ || y < 1 || y > n_) ? E
                                              : board_[(y - 1) * n_ + (x - 1)];
}

void Board::set(int& x, int& y, State s)
{
  assert(x > 0 && x <= n_ && y > 0 && y <= n_);
  board_[(y - 1) * n_ + (x - 1)] = s;
}

int Board::contact(int x, int y) const
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

void Board::infection(double ratInf)
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

void Board::swap(int x, int y)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(-1, 1);

  int i = dis(gen);
  int j = dis(gen);
  int x2 = x + i;
  int y2 = y + j;
  if (((i != 0) || (j != 0)) && x2 > 0 && x2 <= n_ && y2 > 0 && y2 <= n_ &&
      get(x2, y2) == E) {
    set(x2, y2, get(x, y));
    set(x, y, E);
    stay_[(y2 - 1) * n_ + (x2 - 1)] = stay_[(y - 1) * n_ + (x - 1)];
    stay_[(y - 1) * n_ + (x - 1)] = 0;
  }
}

Board Board::epidemic(double pInf, int tMean)
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
      if (board_[coordinate] != E) {
        next.swap(x, y);
      }
    }
  }

  sit.t = ++evolution_.back().t;
  next.evolution_.push_back({sit});

  return next;
}

Board Board::epidemic2(double pInf, double pGua)
{
  assert(pInf > 0 && pInf < 1 && pGua > 0 && pGua < 1);
  Board next(n_, density_);
  next.evolution_ = evolution_;
  next.stay_ = stay_;
  Situation sit = {0, 0, 0, 0};

  for (int x = 1; x != n_ + 1; ++x) {
    for (int y = 1; y != n_ + 1; ++y) {
      int coordinate = (y - 1) * n_ + (x - 1);

      if (board_[coordinate] == S) {
        ++sit.s;
        int infected = contact(x, y);
        int infections = 0;

        for (int i = 0; i != infected; ++i) {
          std::random_device rd;
          std::mt19937 gen(rd());
          std::uniform_real_distribution<> dis(0.0, 1.0);
          double probability = dis(gen);
          if (probability <= pInf) {
            ++infections;
          }
        }
        if (infections > 0) {
          next.set(x, y, I);
        } else {
          next.set(x, y, S);
        }
      }
      if (board_[coordinate] == I) {
        ++sit.i;
        ++next.stay_[coordinate];

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0., 1.0);

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
      if (board_[coordinate] != E) {
        next.swap(x, y);
      }
    }
  }
  sit.t = ++evolution_.back().t;
  next.evolution_.push_back({sit});

  return next;
}

representBoard Board::Draw(int const& quadSize)
{
  representBoard rappresentation(quadSize, board_);
  return rappresentation;
}

double Board::tMean() const
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

void Board::trend() const
{
  std::ofstream fout;
  fout.open("trend.txt");
  for (auto& v : evolution_) {
    fout << std::setw(4) << v.t << ' ' << std::setw(4) << v.s << ' '
         << std::setw(4) << v.i << ' ' << std::setw(4) << v.r << ' '
         << std::setw(4) << tMean() << ' ' << std::setw(4) << v.i + v.r + v.s
         << '\n';
  }

  fout.close();
}

bool Board::count() const
{
  int i = 0;
  for (auto const& v : board_) {
    if (v == I) {
      ++i;
    }
  }
  return !i;  // piÃ¹ easy, non serve piÃ¹ il resto
}

void Board::parameters()
{
  std::vector<double> beta;
  std::vector<double> gamma;
  for (unsigned int i = 0; i != evolution_.size() - 1;
       ++i) {  // le formule sono con le percentuali
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
  for (unsigned int i = 0; i != beta.size(); ++i) {
    std::cout << beta[i] << ' ' << gamma[i] << '\n';
  }
}