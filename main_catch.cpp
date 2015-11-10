#define CATCH_CONFIG_MAIN // This tells Catch to provide a main()
#include <array>
#include <vector>
#include <catch.hpp>
#include <boost/numeric/odeint.hpp>
#include <boost/range.hpp>
#include <boost/range/adaptor/sliced.hpp>
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

typedef boost::iterator_range<std::vector<int>::iterator> vi_range;
int my_range_sum(const vi_range &c) {
  int sum = 0;
  for (auto it = boost::begin(c); it != boost::end(c); ++it) {
    sum += *it;
  }
  return sum;
}

int my_range_sum(const boost::iterator_range<int *> &c) {
  std::vector<int> v(c.begin(), c.end());
  return my_range_sum(v);
}

vi_range &my_one_maker(vi_range &c) {
  // Range currying
  for (auto it = boost::begin(c); it != boost::end(c); ++it) {
    *it = 1;
  }
  return c;
}

vi_range actually_const_but_not_labeled(vi_range &c) {
  // currying
  // c++;
  return c;
}

TEST_CASE("boost range experiments") {
  vector<int> v{{2, 3, 4, 5}};
  // https://tlzprgmr.wordpress.com/2008/06/04/c-using-boost-ranges-to-simplify-enumerations/
  // decent range example and constness explanation in bug report
  // https://svn.boost.org/trac/boost/ticket/10514
  // slicing also really cool:
  // http://www.boost.org/doc/libs/1_51_0/libs/range/doc/html/range/reference/adaptors/reference/sliced.html
  // more explanations:
  // http://theboostcpplibraries.com/boost.range-helper-classes-and-functions

  SECTION("vector is cast to range") {
    auto sum = my_range_sum(v);
    CHECK(14 == sum);
  }

  SECTION("const range can still be added but not modified") {
    // boost::sub_range<const int> const_range = boost::make_iterator_range(v);
    const vi_range new_range = boost::make_iterator_range(v);
    auto sum = my_range_sum(new_range);
    CHECK(14 == sum);

    // actually_const_but_not_labeled(new_range);    //correctly prevents access
  }

  SECTION("sub_range creation is somewhat more convenient but works with same "
          "funcs") {
    const boost::sub_range<std::vector<int>> const_sub(boost::begin(v),
                                                       boost::end(v));
    auto sum = my_range_sum(const_sub);
    CHECK(14 == sum);

    // actually_const_but_not_labeled(const_sub);    //correctly prevents access
  }

  SECTION("non-const ranges allow modification") {
    auto range = boost::make_iterator_range(v);
    int sum = my_range_sum(my_one_maker(range));
    CHECK(4 == sum);
  }

  SECTION("slicing does not change type") {
    int sum = my_range_sum(v | boost::adaptors::sliced(1,3));
    CHECK(7 == sum);
  }

  SECTION("range can also be sliced") {
    auto range = boost::make_iterator_range(v);
    int sum = my_range_sum(range | boost::adaptors::sliced(1,3));
    CHECK(7 == sum);
  }

  SECTION("const range works on temporary ranges") {
    int sum = my_range_sum(boost::make_iterator_range(v));
    CHECK(14 == sum);
  }

  SECTION("Range provides random access and slicing") {
    auto my_range = boost::make_iterator_range(v.begin() + 1, v.end());
    CHECK(4 == my_range[1]);
  }

  SECTION("I can create range from pointers") {
    auto range = boost::make_iterator_range_n(v.data(), v.size());
    CHECK(4 == range[2]);
    auto sum = my_range_sum(range); //this is not a vector iterator range anymore; needs adapter
    CHECK(14 == sum);
  }

  SECTION("Pointer range is still randomly accessible") {
    auto range = boost::make_iterator_range_n(v.data(), v.size());
    range[2] = 15;
    CHECK(range[2] == 15);
  }
}

TEST_CASE("can oclint detect out of bound access") {
  // trying static analysis
  vector<int> v(3, 0);

  int sum = 0;
  sum += v[4];

  CHECK(0 != sum);
}
