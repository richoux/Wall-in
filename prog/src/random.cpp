#include "../include/random.hpp"

namespace wallin
{
  Random::Random()
  {
    rng.seed( time(NULL) + getpid() + getppid() );
  }
}
