#pragma once

#include <random>
#include <fstream>

namespace wallin
{
  class Random
  {
  public:
    Random();
    Random(const Random&) = default;
    Random(Random&&) = default;
    Random& operator=(const Random&) = default;
    Random& operator=(Random&&) = default;

    inline int getRandNum( int limit ) { return ( numbers(rng) % limit ); } 
  private:
    std::mt19937			rng;
    //std::random_device			rd;
    unsigned int seed;
    std::uniform_int_distribution<int>  numbers;
  };
}
