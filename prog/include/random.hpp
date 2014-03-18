#pragma once

#include <random>

namespace wallin
{
  class Random
  {
  public:
    Random() : rng( rd() ){ };
    Random(const Random&) = delete;
    Random(Random&&) = delete;
    Random& operator=(const Random&) = delete;
    Random& operator=(Random&&) = delete;

    inline int getRandNum( int limit ) { return ( numbers(rng) % limit ); } 
  private:
    std::mt19937			rng;
    std::random_device			rd;
    std::uniform_int_distribution<int>  numbers;
  };
}
