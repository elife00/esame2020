#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "functions.h"

TEST_CASE("testing factorial")
{
  CHECK(factorial(0) == 1);
  CHECK(factorial(1) == 1);
  CHECK(factorial(2) == 2);
  CHECK(factorial(10) == 3628800);
}

TEST_CASE("testing cumulative")
{
  CHECK(cumulative(6, 10) == doctest::Approx(0.130).epsilon(0.001));
  CHECK(cumulative(10, 10) == doctest::Approx(0.583).epsilon(0.001));
  CHECK(cumulative(15, 10) == doctest::Approx(0.951).epsilon(0.001));
  CHECK(cumulative(20, 10) == doctest::Approx(0.998).epsilon(0.001));
  CHECK(cumulative(10, 15) == doctest::Approx(0.118).epsilon(0.001));
}
