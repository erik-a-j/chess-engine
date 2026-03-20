#ifndef RNG_H
#define RNG_H

#include <cstdint>

namespace chess::rng {

class SparseRNG {
    uint64_t state;

  public:
    SparseRNG(uint64_t seed) : state{seed} {}

    uint64_t generate()
    {
        state ^= state >> 12;
        state ^= state << 25;
        state ^= state >> 27;
        uint64_t r1 = state * 0x2545F4914F6CDD1DULL;
        state ^= state >> 12;
        state ^= state << 25;
        state ^= state >> 27;
        uint64_t r2 = state * 0x2545F4914F6CDD1DULL;
        state ^= state >> 12;
        state ^= state << 25;
        state ^= state >> 27;
        uint64_t r3 = state * 0x2545F4914F6CDD1DULL;
        return r1 & r2 & r3;
    }
};

}  // namespace chess::rng

#endif /* #ifndef RNG_H */
