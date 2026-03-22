#ifndef BITUTIL_H
#define BITUTIL_H

#if __has_include(<bit>)
# include <bit>
#endif
#include <cstdint>

namespace chess::bitutil {

constexpr int popcount(uint64_t v) noexcept
{
#ifdef __cpp_lib_bitops
    return std::popcount(v);
#else
    return __builtin_popcountll(v);
#endif
}

constexpr int lsb(uint64_t v) noexcept
{
#ifdef __cpp_lib_bitops
    return std::countr_zero(v);
#else
    return v ? __builtin_ctzll(v) : 64;
#endif
}

constexpr int msb(uint64_t v) noexcept
{
#ifdef __cpp_lib_bitops
    return std::countl_zero(v);
#else
    return v ? __builtin_clzll(v) : 64;
#endif
}

constexpr int pop_lsb(uint64_t& v) noexcept
{
    const int i = lsb(v);
    v &= v - 1;
    return i;
}

}  // namespace chess::bitutil

#endif /* #ifndef BITUTIL_H */
