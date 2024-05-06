#include <iostream>

#include "Singleton.hpp"

using si = pair_singleton<int, int>; // Alias for pair_singleton<int, int>

int main()
{
    si x(std::make_pair(19, 1)); // Construct x with a pair<int, int>
    si y(std::make_pair(12, 12)); // Construct y with a pair<int, int>

    std::cout << "x == x: " << (x == x) << std::endl;
    std::cout << "x < y: " << (x < y) << std::endl;

    return 0;
}
