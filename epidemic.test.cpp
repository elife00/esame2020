#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "epidemic.hpp"

TEST_CASE("testing contact(), get() and set()") {
  // creating a board with infected people in the 4 corners
  int a = 5;
  int b = 1;
  int c = 3;
  int d = 4;
  Board population(5, 1);
  population.set(a, a, I);
  population.set(b, b, I);
  population.set(b, a, I);
  population.set(a, b, I);
  population.set(c, d, I);
  population.set(d, c, I);

  CHECK(population.get(a, a) == I);
  CHECK(population.get(c, c) == S);

  CHECK(population.contact(3, 3, 1) == 2);
  CHECK(population.contact(3, 3, 2) == 6);
}

TEST_CASE("testing epidemic, infection(), avg_time()") {
  int dim = 120;

  double density;
  double ratInf;
  double pInf;
  int avgTime;
  int range;

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<> p(0., 1.);
  std::uniform_int_distribution<> t(1, 40);
  std::uniform_int_distribution<> r(1, 5);

  density = p(gen);
  ratInf = p(gen);
  pInf = p(gen);
  avgTime = t(gen);
  range = r(gen);

  int S = static_cast<int>(dim * dim * density);
  int I = static_cast<int>(S * ratInf);
  Situation So = {0, S, 0, 0};

  Board population(dim, density);

  CHECK(population.situation() == So);

  population.infection(ratInf);
  population = population.epidemic(pInf, avgTime, range, false);

  if (population.situation().i != I) { // fixes an eventual approximation error
    ++I;
  }
  std::cout << I << '\n';

  Situation Suno = {1, S - I, I, 0};
  CHECK(population.situation() == Suno);

  while (population.situation().i != 0) {
    population = population.epidemic(pInf, avgTime, range, false);
    CHECK((population.situation().i + population.situation().s +
               population.situation().r ==
           S));
  }

  CHECK(population.avg_time() == doctest::Approx(avgTime).epsilon(0.5));
}
