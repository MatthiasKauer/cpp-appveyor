// #define CATCH_CONFIG_MAIN // This tells Catch to provide a main()
// #include <catch.hpp>

#include <iostream>

int non_const_func(int &x_) {
  return x_;
}

template<typename T>
T non_const_template_func(T &x_) {
  return x_;
}

int main() {
//Can I call non-const function with constant object
  const int x = 3;
  // int y = non_const_func(x);
  int y = non_const_template_func(x);

  std::cout << y << std::endl;
}

