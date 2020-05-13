#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
#include <chrono>

struct Situation
{
  using namespace std::literals::chrono_literals;
  double s = 0;
  double i = 0;
  double r = 0;
  std::chrono::seconds t = 0;
};

class Epidemic
{
 private:
  double const beta_;  // p*c = tasso effettivo di contatto -> pci = probabilità
                       // di diventare infetto/dt
  double const gamma_;  // 1/tau -> tasso/frequenza di rimozione
  std::vector<Situation> evolution_;

 public:
  Epidemic(double i, double beta, double gamma)
      : beta_(beta), gamma_(gamma), evolution_{{1 - i, i, 0, 0}}
  {
    assert(i > 0 && i <= 0.1);
  }
  // i = percentuale di infetti

  void evolve(double days, double points)
  {
    double dt = days / points;
    for (int i = 0; i != points; ++i) {
      auto back = evolution_.back();
      double S = back.s - beta_ * back.s * back.i * dt;
      double I = back.i + (beta_ * back.s * back.i - gamma_ * back.i) * dt;
      double R = back.r + gamma_ * back.i * dt;
      double t = back.t += dt;
      evolution_.push_back({S, I, R, t});
    }
  }

  double get_time()
  {
    return evolution_.back().t;
  }

  std::vector<Situation> evolution() const
  {
    return evolution_;
  }

  void print()
  {
    
    for (auto v : evolution_) {
      std::cout.precision(4);
      std::cout.width(8);
      std::cout << v.t;
      std::cout.width(9);
      std::cout << v.s;
      std::cout.width(8);
      std::cout << v.i;
      std::cout.width(8);
      std::cout << v.r << '\n';
    }
  }
};

int main()
{
  Epidemic Covid(0.1, 0.131, 0.0351);
  Covid.evolve(80, 200);
  Covid.print();
}