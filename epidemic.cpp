#include "epidemic.hpp"

std::vector<Situation> Board::evolution_{};

State Board::get(int x, int y) const {
  return (x < 1 || x > n_ || y < 1 || y > n_) ? E
                                              : board_[(y - 1) * n_ + (x - 1)];
}

void Board::set(int x, int y, State s) {
  assert(x > 0 && x <= n_ && y > 0 && y <= n_);
  board_[(y - 1) * n_ + (x - 1)] = s;
}

int Board::contact(int x, int y, int r) const {
  assert(x > 0 && x <= n_ && y > 0 && y <= n_ && r > 0 && r <= n_);
  int count = 0;
  for (int i = (-r); i <= r; ++i) {
    for (int j = (-r); j <= r; ++j) {
      if (get(x + i, y + j) == I) {
        ++count;
      }
    }
  }
  return count;
}

void Board::infection(double ratInf) {
  assert(ratInf > 0 && ratInf < 1);
  int infected = static_cast<int>(ratInf * density_ * n_ * n_);
  for (int i = 0, j = 0; i != infected; ++j) {
    if (board_[j] == S) {
      board_[j] = I;
      ++i;
    }
  }
  std::random_device seed;
  std::mt19937 g(seed());
  std::shuffle(board_.begin(), board_.end(), g);
}

void Board::swap(int x, int y) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(-1, 1);

  int i = dis(gen);
  int j = dis(gen);
  int x2 = x + i;
  int y2 = y + j;
  if (((i != 0) || (j != 0)) && x2 > 0 && x2 <= n_ && y2 > 0 &&
      y2 <= n_ && // if i = j = 0 the cell doesn't move
      get(x2, y2) == E) {
    set(x2, y2, get(x, y));
    set(x, y, E);
    stay_[(y2 - 1) * n_ + (x2 - 1)] = stay_[(y - 1) * n_ + (x - 1)];
    stay_[(y - 1) * n_ + (x - 1)] = 0;
  }
}

Board Board::epidemic(double pInf, int avgTime, int range, bool quarantine) {
  assert(pInf > 0 && pInf < 1 && avgTime > 0 && avgTime < 40 && range > 0);
  /*Situation sit = {0, 0, 0, 0};*/  Situation sit = {sit.t = ++evolution_.back().t + 1, 0, 0, 0};
  Board next(n_, density_);
  //next.evolution_ = evolution_;  /*static evolution*/
  next.stay_ = stay_;

  std::random_device rd;
  std::mt19937 gen(rd());
  
  std::uniform_real_distribution<> u_dis(0., 1.0); // portata fuori

  for (int x = 1; x != n_ + 1; ++x) {
    for (int y = 1; y != n_ + 1; ++y) {
      int coordinate = (y - 1) * n_ + (x - 1);

      if (board_[coordinate] == S) {
        ++sit.s;
        int infected = contact(x, y, range);
          
        std::binomial_distribution<> bin_dis(infected, pInf); //cannot be outside

        if (bin_dis(gen) > 0) {
          next.set(x, y, I);
        } else {
          next.set(x, y, S);
        }
      }
      if (board_[coordinate] == I) {
        ++sit.i;
          ++next.stay_[coordinate];

        int iDays = next.stay_[coordinate];

        if (u_dis(gen) <= 1. / avgTime) {
          next.set(x, y, R);
        } else if (iDays >= 0.5 * avgTime && quarantine) {
          next.set(x, y, Q);
        } else {
          next.set(x, y, I);
        }
      }
      if (board_[coordinate] == R) {
        ++sit.r;
        next.set(x, y, R);
      }
      if (board_[coordinate] == Q) {
        ++sit.i;
        ++next.stay_[coordinate];

        if (u_dis(gen) <= 1. / avgTime) {
          next.set(x, y, R);
        } else {
          next.set(x, y, Q);
        }
      }
      if (board_[coordinate] == E) {
        next.set(x, y, E);
      }
    }
  }

  for (int x = 1; x != n_ + 1; ++x) {
    for (int y = 1; y != n_ + 1; ++y) {
      int coordinate = (y - 1) * n_ + (x - 1);
      if (board_[coordinate] != E && board_[coordinate] != Q) {
        next.swap(x, y); 
      }
    }
  }

  sit.t = ++evolution_.back().t;
  //next.evolution_.push_back(sit);
  Board::evolution_.back() = sit;

  return next;
}

representBoard Board::draw() { // we need access to board_
  representBoard rappresentation(board_);
  return rappresentation;
}

double Board::avg_time() {
  int i = 0.;
  int a = std::accumulate(stay_.begin(), stay_.end(), 0);
  //i = std::count_if(stay_.begin(), stay_.end(), [](int i){return i != 0;});
  
  for (auto v : stay_) {
    if (v != 0) {
      ++i;
    }
  }
  
  return static_cast<double> (a) / i;
}

void Board::trend() {
  std::ofstream fout;
  fout.open("trend.txt");
  for (auto &v : evolution_) {
    fout << std::setw(4) << v.t << ' ' << std::setw(4) << v.s << ' '
         << std::setw(4) << v.i << ' ' << std::setw(4) << v.r << ' '
         << std::setw(4) << v.i + v.r + v.s << '\n';
  }
  fout << "The average time of recovery is : " << std::setw(4) << avg_time();
  fout.close();
}

Situation Board::situation() const { return evolution_.back(); } 

bool Board::end() { // ERRORE: abbiamo usato un int ma doveva essere un bool
  bool i = true;
  for (auto const &v : board_) {
      if (v == I || v == Q) {
          i = false;
          break;
      }
  }
  return i;
}
