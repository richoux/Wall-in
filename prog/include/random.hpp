#pragma once

#include <random>
#include <unistd.h>

namespace wallin
{
  class Random
  {
  public:
    Random();
    Random(const Random&) = default;
    Random(Random&&) noexcept = default;
    Random& operator=(const Random&) = default;
    Random& operator=(Random&&) noexcept = default;
    ~Random() = default;

    inline int getRandNum( int limit ) { return ( numbers(rng) % limit ); } 
  private:
    std::random_device			rd;
    std::mt19937			rng;
    std::uniform_int_distribution<int>  numbers;
  };
}
