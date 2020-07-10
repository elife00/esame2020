//
//  epidemic.test.cpp
//
//
//  Created by Michele Mattei on 04/07/2020.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "epidemic.hpp"

/*TEST_CASE("testing epidemic") {
  int dim = 120;
  double density[5] = {0.15, 0.25, 0.5, 0.75, 0.95};
  double ratInf[5] = {0.001, 0.05, 0.01, 0.2, 0.0003};
  int avgTime[5] = {8, 10, 15, 20, 32};
  int range[5] = {5, 3, 2, 1, 4};
  for (int i = 0; i != 5; ++i) {
    double d = density[i];
    double r = ratInf[i];
    int t = avgTime[i];
    int S = static_cast<int> (dim * dim * d);
    int I = static_cast<int> (S * r);

      Situation So = {0, S, 0, 0};
      Situation Suno = {0, S-I, I, 0};

    Board population(dim, d);

    CHECK(population.current_situation() == So);
    population.infection(r);
    CHECK(population.current_situation() == Suno);
    while (population.current_situation().i != 0) {
      population = population.epidemic(r, t, range[i], false);
      CHECK((population.current_situation().i +
             population.current_situation().s +
             population.current_situation().r) == S);
    }
  }
}*/

/*TEST_CASE("testing epidemic, infection(), avg_time()") {
  int dim = 120;

  double density;
  double ratInf;
  double pInf;
  int avgTime;
  int range;

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<> p(0, 1);
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
  CHECK(population.current_situation() == So);

  population.infection(ratInf);
  if (population.current_situation().i !=
      I) { // fixes an eventual approximation error
    ++I;
  }
  // is 0 because the increase begins from the following iteration
  Situation Suno = {0, S - I, I, 0};

  CHECK(population.current_situation() == Suno);

  while (population.current_situation().i != 0) {
    population = population.epidemic(pInf, avgTime, range, false);
    CHECK((population.current_situation().i + population.current_situation().s +
           population.current_situation().r) == S);
  }

  CHECK(population.avg_time() == doctest::Approx(tAvg).epsilon(0.5));
}*/

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
  
  CHECK(population.get(a,a) == I);
  CHECK(population.get(c,c) == S);

  CHECK(population.contact(3, 3, 1) == 2);
  CHECK(population.contact(3, 3, 2) == 6);
  
}
