#include <iostream>

#include "MemoryParser.h"

int main() {
    auto mem = GetMemoryInfo();

    for (auto it = mem.begin(); it != mem.end(); it++) {
        std::cout << it->first << " " << it->second << std::endl;
    }
}