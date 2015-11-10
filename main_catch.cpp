#define CATCH_CONFIG_MAIN // This tells Catch to provide a main()
#include <array>
#include <vector>
#include <catch.hpp>
#include <boost/numeric/odeint.hpp>
#include <boost/range.hpp>
//#include <boost/range/iterator.hpp>

// #include "build/installed/catch.hpp"

using namespace std;
using namespace boost::numeric::odeint;

unsigned int Factorial(unsigned int number) {
  return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Factorials are computed", "[factorial]") {
  REQUIRE(Factorial(1) == 1);
  REQUIRE(Factorial(2) == 2);
  REQUIRE(Factorial(3) == 6);
  REQUIRE(Factorial(10) == 3628800);
}

// http://headmyshoulder.github.io/odeint-v2/examples.html
const double sigma = 10.0;
const double R = 28.0;
const double b = 8.0 / 3.0;

typedef std::array<double, 3> state_type;

void lorenz(const state_type &x, state_type &dxdt, double /*t */) {
  dxdt[0] = sigma * (x[1] - x[0]);
  dxdt[1] = R * x[0] - x[1] - x[0] * x[2];
  dxdt[2] = -b * x[2] + x[0] * x[1];
}

void write_lorenz(const state_type &x, const double t) {
  cout << t << '\t' << x[0] << '\t' << x[1] << '\t' << x[2] << endl;
}

TEST_CASE("Odeint works", "[odeint]") {
  state_type x = {10.0, 1.0, 1.0}; // initial conditions
  auto x1 = x;
  integrate(lorenz, x, 0.0, 25.0, 0.1, write_lorenz);

  REQUIRE(x1[0] != x[0]);
}

template <class Range> int my_range_sum(const Range &c) {
  int sum = 0;
  for (auto it = boost::begin(c); it != boost::end(c); ++it) {
    sum += *it;
  }
  return sum;
}

template <class Range> Range &my_one_maker(Range &c) {
  // Range currying
  for (auto it = boost::begin(c); it != boost::end(c); ++it) {
    *it = 1;
  }
  return c;
}

TEST_CASE("boost range experiments") {
  vector<int> v{{2, 3, 4, 5}};
  // https://tlzprgmr.wordpress.com/2008/06/04/c-using-boost-ranges-to-simplify-enumerations/
  auto sum = my_range_sum(v);
  CHECK(14 == sum);

  sum = my_range_sum(boost::make_iterator_range(v));
  CHECK(14 == sum);

  auto my_range = boost::make_iterator_range(v.begin() + 1, v.end());
  sum = my_range_sum(my_range);
  CHECK(12 == sum);

  sum = my_range_sum(my_one_maker(my_range));
  CHECK(3 == sum);
}
