

#include <iostream>
#include <vector>

int main() {
  std::vector<int> thisismyint = {1, 2, 3, 4};

  for (int &i : thisismyint) {
    std::cout << i << std::endl;
  }

  std::cout << "\n";

  thisismyint.erase(thisismyint.begin());

  for (int &i : thisismyint) {
    std::cout << i << std::endl;
  }

  std::cin.get();
}
