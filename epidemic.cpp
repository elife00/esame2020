#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

enum class State : char { Susceptible, Infected, Recovered };
constexpr State S = State::Susceptible;
constexpr State I = State::Infected;
constexpr State R = State::Recovered;

struct Situation
{
  int t = 0;
  int s = 0;
  int i = 0;
  int r = 0;
};

///

class Population
{
  int n_;
  std::vector<State> population_;
  std::vector<unsigned int> stay_;
  std::vector<Situation> evolution_;

 public:
  Population(int const& n)
      : n_{n}
      , population_(n * n, S)
      , stay_(n_ * n_, 0)
      , evolution_{{0, n * n, 0, 0}}
  {
  }

  std::vector<State>& vector()
  {
    return population_;
  }

  State& set(int& x, int& y, State s)
  {
    assert(x > 0 && x <= n_ && y > 0 && y <= n_);
    population_[(x - 1) + (y - 1) * n_] = s;
  }

  State get(int const& x, int const& y) const
  {
    return (x < 0 || x >= n_ || y < 0 || y >= n_)
               ? S
               : population_[(x - 1) + (y - 1) * n_];
  }

  auto infection(double const& ratInf)
  {
    for (int i = 0; i != n_ * n_ * ratInf; ++i) {
      population_[i] = I;
    }
    std::random_device sed;
    std::mt19937 gen(sed());
    std::shuffle(population_.begin(), population_.end(), gen);
  }

  int contact(int const& x, int const& y) const
  {
    assert(x > 0 && x <= n_ && y > 0 && y <= n_);

    int infected = 0;

    for (int i = x - 1; i != x + 2; ++i) {
      for (int j = y - 1; j != y + 2; ++j) {
        if (x < 0 || x >= n_ || y < 0 || y >= n_) {
        } else if (population_[(x - 1) + (y - 1) * n_] == I) {
          ++infected;
        }
      }
    }
    assert(infected < 9 && infected > -1);
    return infected;
  }

  Population epidemic(double const& pInf, double const& pGua) const
  {
    assert(pInf > 0 && pInf < 1 && pGua > 0 && pGua < 1);
    Population next(n_);
    next.evolution_ = evolution_;
    next.stay_ = stay_;
    Situation sit = {0, 0, 0, 0};

    for (int x = 1; x != n_ + 1; ++x) {
      for (int y = 1; y != n_ + 1; ++y) {
        int coordinate = (x - 1) + (y - 1) * n_;
        if (population_[coordinate] == S) {
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

        if (population_[coordinate] == I) {
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

        if (population_[coordinate] == R) {
          ++sit.r;
          next.set(x, y, R);
        }
      }
    }

    sit.t = evolution_.back().t + 1;
    next.evolution_.push_back({sit});
    return next;
  }

  void draw() const
  {
    std::string result;
    int i = 0;
    for (auto const& v : population_) {
      std::string symbol;
      switch (v) {
        case S:
          symbol = '|';
          break;
        case I:
          symbol = "\033[1;31m|\033[0m";
          break;
        case R:
          symbol = ' ';
          break;
      }
      if (i % n_ == 0) {
        result += '\n';
        result += symbol;
        result += ' ';
      } else {
        result += symbol;
        result += ' ';
      }
      ++i;
    }
    std::cout << result << '\n';
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

  void trend() const
  {
    /*std::cout << std::setw(4) << "days" << ' ' << std::setw(4) << "S" << ' '
              << std::setw(4) << "I" << ' ' << std::setw(4) << "R" << '\n';*/
    for (auto& v : evolution_) {
      std::cout << std::setw(4) << v.t << ' ' << std::setw(4) << v.s << ' '
                << std::setw(4) << v.i << ' ' << std::setw(4) << v.r << '\n';
    }
  }

  bool count() const
  {
    int I = 0;
    int S = 0;
    int R = 0;
    for (auto const& v : population_) {
      if (v == S) {
        ++S;
      } else if (v == I) {
        ++I;
      } else if (v == R) {
        ++R;
      }
    }
    if (I == 0) {
      return true;
    }

    std::cout << "S = " << S << '\n';
    std::cout << "I = " << I << '\n';
    std::cout << "R = " << R << '\n';
    std::cout << "N = " << S + R + I << '\n';

    return false;
  }

  void parameters()
  {
    std::vector<double> beta;
    std::vector<double> gamma;
    for (int i = 0; i != evolution_.size() - 1;
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
    for (int i = 0; i != beta.size(); ++i) {
      std::cout << beta[i] << ' ' << gamma[i] << '\n';
    }
  }
};

int main()
{
  int dim = 10;
  double pInf = 0.15;
  double pGua = 0.1;  // 1 giorno = 100 milliseconds
  double ratInf = 0.2;

  Population population(dim);
  std::cout << "How an epidemic begin:\n";

  population.infection(ratInf);
  population.draw();
  population.count();

  std::cout << '\n' << "Epidemic has began\n";

  std::this_thread::sleep_for(std::chrono::seconds(5));

  int days = 0;
  while (true) {
    ++days;
    std::cout << "\033c";
    population = population.epidemic(pInf, pGua);
    population.draw();

    if (population.count()) {
      break;
    }
    std::cout << "giorni dall'inizio dell'epidemia: " << days << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  population.trend();
  std::cout << '\n' << '\n';
  population.parameters();
}