#ifndef CONSTANT_H
#define CONSTANT_H

#include "types.h"

namespace chess {

#include "generated/precomputed.h"

// clang-format off
constexpr good_magic_t good_magics[][64] = {
    /* BISHOPS */
    {
        /*A1*/ {0xffedf9fd7cfcffffULL, 5}, /*B1*/ {0xfc0962854a77f576ULL, 4}, /*C1*/ {0, 0}, /*D1*/ {0, 0}, 
        /*E1*/ {0, 0}, /*F1*/ {0, 0}, /*G1*/ {0xfc0a66c64a7ef576ULL, 4}, /*H1*/ {0x7ffdfdfcbd79ffffULL, 5},
        /*A2*/ {0xfc0846a64a34fff6ULL, 4}, /*B2*/ {0xfc087a874a3cf7f6ULL, 4}, /*C2*/ {0, 0}, /*D2*/ {0, 0}, 
        /*E2*/ {0, 0}, /*F2*/ {0, 0}, /*G2*/ {0xfc0864ae59b4ff76ULL, 4}, /*H2*/ {0x3c0860af4b35ff76ULL, 4},
        /*A3*/ {0x73C01AF56CF4CFFBULL, 4}, /*B3*/ {0x41A01CFAD64AAFFCULL, 4}, /*C3*/ {0, 0}, /*D3*/ {0, 0}, 
        /*E3*/ {0, 0}, /*F3*/ {0, 0}, /*G3*/ {0x7c0c028f5b34ff76ULL, 4}, /*H3*/ {0xfc0a028e5ab4df76ULL, 4},
        /*A4*/ {0, 0}, /*B4*/ {0, 0}, /*C4*/ {0, 0}, /*D4*/ {0, 0}, /*E4*/ {0, 0}, /*F4*/ {0, 0}, /*G4*/ {0, 0}, /*H4*/ {0, 0},
        /*A5*/ {0, 0}, /*B5*/ {0, 0}, /*C5*/ {0, 0}, /*D5*/ {0, 0}, /*E5*/ {0, 0}, /*F5*/ {0, 0}, /*G5*/ {0, 0}, /*H5*/ {0, 0},
        /*A6*/ {0xDCEFD9B54BFCC09FULL, 4}, /*B6*/ {0xF95FFA765AFD602BULL, 4}, /*C6*/ {0, 0}, /*D6*/ {0, 0}, 
        /*E6*/ {0, 0}, /*F6*/ {0, 0}, /*G6*/ {0x43ff9a5cf4ca0c01ULL, 4}, /*H6*/ {0x4BFFCD8E7C587601ULL, 4},
        /*A7*/ {0xfc0ff2865334f576ULL, 4}, /*B7*/ {0xfc0bf6ce5924f576ULL, 4}, /*C7*/ {0, 0}, /*D7*/ {0, 0}, 
        /*E7*/ {0, 0}, /*F7*/ {0, 0}, /*G7*/ {0xc3ffb7dc36ca8c89ULL, 4}, /*H7*/ {0xc3ff8a54f4ca2c89ULL, 4},
        /*A8*/ {0xfffffcfcfd79edffULL, 5}, /*B8*/ {0xfc0863fccb147576ULL, 4}, /*C8*/ {0, 0}, /*D8*/ {0, 0}, 
        /*E8*/ {0, 0}, /*F8*/ {0, 0}, /*G8*/ {0xfc087e8e4bb2f736ULL, 4}, /*H8*/ {0x43ff9e4ef4ca2c89ULL, 5},
    },

    /* ROOKS */
    {
        /*A1*/ {0, 0}, /*B1*/ {0, 0}, /*C1*/ {0, 0}, /*D1*/ {0, 0}, /*E1*/ {0, 0}, /*F1*/ {0, 0}, /*G1*/ {0, 0}, /*H1*/ {0, 0},
        /*A2*/ {0, 0}, /*B2*/ {0, 0}, /*C2*/ {0, 0}, /*D2*/ {0, 0}, /*E2*/ {0, 0}, /*F2*/ {0, 0}, /*G2*/ {0, 0}, /*H2*/ {0, 0},
        /*A3*/ {0, 0}, /*B3*/ {0, 0}, /*C3*/ {0, 0}, /*D3*/ {0, 0}, /*E3*/ {0, 0}, /*F3*/ {0, 0}, /*G3*/ {0, 0}, /*H3*/ {0, 0},
        /*A4*/ {0, 0}, /*B4*/ {0, 0}, /*C4*/ {0, 0}, /*D4*/ {0, 0}, /*E4*/ {0, 0}, /*F4*/ {0, 0}, /*G4*/ {0, 0}, /*H4*/ {0, 0},
        /*A5*/ {0, 0}, /*B5*/ {0, 0}, /*C5*/ {0, 0}, /*D5*/ {0, 0}, /*E5*/ {0, 0}, /*F5*/ {0, 0}, /*G5*/ {0, 0}, /*H5*/ {0, 0},
        /*A6*/ {0, 0}, /*B6*/ {0, 0}, /*C6*/ {0, 0}, /*D6*/ {0, 0}, /*E6*/ {0, 0}, /*F6*/ {0, 0}, /*G6*/ {0, 0}, /*H6*/ {0, 0},
        /*A7*/ {0x48FFFE99FECFAA00ULL, 10}, /*B7*/ {0x48FFFE99FECFAA00ULL, 9}, /*C7*/ {0x497FFFADFF9C2E00ULL, 9}, /*D7*/ {0x613FFFDDFFCE9200ULL, 9},
        /*E7*/ {0xffffffe9ffe7ce00ULL, 9}, /*F7*/ {0xfffffff5fff3e600ULL, 9}, /*G7*/ {0x0003ff95e5e6a4c0ULL, 9}, /*H7*/ {0x510FFFF5F63C96A0ULL, 10},
        /*A8*/ {0xEBFFFFB9FF9FC526ULL, 11}, /*B8*/ {0x61FFFEDDFEEDAEAEULL, 10}, /*C8*/ {0x53BFFFEDFFDEB1A2ULL, 10}, /*D8*/ {0x127FFFB9FFDFB5F6ULL, 10},
        /*E8*/ {0x411FFFDDFFDBF4D6ULL, 10}, /*F8*/ {0, 0}, /*G8*/ {0x0003ffef27eebe74ULL, 10}, /*H8*/ {0x7645FFFECBFEA79EULL, 11},
    },

};
// clang-format on

namespace k {

constexpr uint64_t pow(uint64_t base, int exp)
{
    uint64_t r = 1;
    while (exp-- > 0) r *= base;
    return r;
}

consteval size_t rb_table_size(piece_t pt)
{
    size_t s{0};
    const auto& magics = good_magics[pt - piece_t::Bishop];
    const auto& BBbits = pt == piece_t::Bishop ? BBbishop_bits : BBrook_bits;
    for (int i = 0; i < 64; ++i)
    {
        int used_bits = magics[i].magic ? magics[i].used_bits : BBbits[i];
        s += pow(2, used_bits);
    }
    return s;
}

}  // namespace k
}  // namespace chess

#endif /* #ifndef CONSTANT_H */
