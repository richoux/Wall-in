#include "../include/random.hpp"

namespace wallin
{
  Random::Random()
  {
    std::ifstream f("/dev/urandom");
    f.read(reinterpret_cast<char*>(&seed), sizeof(seed)); 
    rng.seed(seed);
  }
}
