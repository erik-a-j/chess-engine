#ifndef TYPES_H
#define TYPES_H

#include <cstddef>
#include <cstdint>
#include "bitutil.h"

namespace chess {

struct square_t {
    enum : int {  // clang-format off
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8,
    };  // clang-format on

    int value{A1};
    constexpr square_t(int v) noexcept : value{v} {}
    constexpr square_t(int file, int rank) noexcept : value{(rank << 3) + file} {}
    static constexpr square_t lsb(uint64_t bb) noexcept { return square_t{bitutil::lsb(bb)}; }
    static constexpr square_t msb(uint64_t bb) noexcept { return square_t{63 ^ bitutil::msb(bb)}; }
    static constexpr square_t pop_lsb(uint64_t& bb) noexcept
    {
        return square_t{bitutil::pop_lsb(bb)};
    }

    constexpr int file() const noexcept { return value & 7; }
    constexpr int rank() const noexcept { return value >> 3; }
    constexpr uint64_t bb() const noexcept { return 1ULL << value; }
    constexpr operator int() const noexcept { return value; }
};

struct piece_t {
    enum : int { None, Pawn, Knight, Bishop, Rook, Queen, King };

    int value{None};
    constexpr piece_t(int v) noexcept : value{v} {}
    constexpr operator int() const noexcept { return value; }
};

struct piece_info_t {
    square_t sq;
    piece_t type;
    bool operator==(const piece_info_t& rhs) const noexcept
    {
        return (sq.value == rhs.sq.value && type.value == rhs.type.value);
    }
};

struct magic_t {
    uint64_t* table;
    uint64_t mask;
    uint64_t magic;
    int shift;

    uint64_t attacks(uint64_t occ) const { return table[((occ & mask) * magic) >> shift]; }
};

struct good_magic_t {
    uint64_t magic;
    int used_bits;
};

}  // namespace chess

#endif /* #ifndef TYPES_H */
