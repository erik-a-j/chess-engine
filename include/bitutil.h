#ifndef BITUTIL_H
#define BITUTIL_H

#include "macro.h"
#if __has_include(<bit>)
# include <bit>
#endif
#include <cstdint>

#ifdef __cpp_lib_bitops
# define bitutil_popcount std::popcount
# define bitutil_lsb      std::countr_zero
#else
# define bitutil_popcount __builtin_popcountll
# define bitutil_lsb      __builtin_ctzll
#endif

namespace chess::bitutil {

constexpr int popcount(uint64_t v) noexcept
{
    return bitutil_popcount(v);
}

constexpr int lsb(uint64_t v) noexcept
{
    return v ? bitutil_lsb(v) : 64;
}

}  // namespace chess::bitutil

#undef bitutil_popcount
#undef bitutil_lsb
#include "macro.h"

#endif /* #ifndef BITUTIL_H */
