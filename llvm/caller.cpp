#include <iostream>

extern "C" {
  int sum(int, int);
}

int main() {
  std::cout << sum(10, 20) << std::endl;
}

