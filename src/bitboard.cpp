/* bitboard.cpp BEGIN */

#include <cstring>
#include <iostream>
#include <sstream>

#include "bitboard.h"
#include "bitutil.h"
#include "rng.h"

namespace chess {

magic_t bishop_magics[64];
magic_t rook_magics[64];
uint64_t bishop_table[bishop_table_SIZE];
uint64_t rook_table[rook_table_SIZE];

static uint64_t rook_mask(square_t sq)
{
    uint64_t mask{0};
    int r = sq.rank();
    int f = sq.file();

    // clang-format off
    for (int ff = f+1; ff<=6; ++ff) mask |= 1ULL << square_t{ff, r};
    for (int ff = f-1; ff>=1; --ff) mask |= 1ULL << square_t{ff, r};
    for (int rr = r+1; rr<=6; ++rr) mask |= 1ULL << square_t{f, rr};
    for (int rr = r-1; rr>=1; --rr) mask |= 1ULL << square_t{f, rr};
    // clang-format on

    return mask;
}
static uint64_t bishop_mask(square_t sq)
{
    uint64_t mask{0};
    int r = sq.rank();
    int f = sq.file();
    int rr, ff;

    // clang-format off
    for (rr = r+1, ff = f+1; rr<=6 && ff<=6; ++rr, ++ff) mask |= 1ULL << square_t{ff, rr};
    for (rr = r+1, ff = f-1; rr<=6 && ff>=1; ++rr, --ff) mask |= 1ULL << square_t{ff, rr};
    for (rr = r-1, ff = f+1; rr>=1 && ff<=6; --rr, ++ff) mask |= 1ULL << square_t{ff, rr};
    for (rr = r-1, ff = f-1; rr>=1 && ff>=1; --rr, --ff) mask |= 1ULL << square_t{ff, rr};
    // clang-format on

    return mask;
}
static uint64_t rook_attacks_slow(square_t sq, uint64_t occ)
{
    uint64_t atk{0};
    int r = sq.rank();
    int f = sq.file();
    int rr, ff;

#define RAY(file, rank) \
    { atk |= 1ULL << square_t{(file), (rank)}; \
      if (occ & (1ULL << square_t{(file), (rank)})) break; }

    // clang-format off
    for (ff = f+1; ff<=7; ++ff) RAY(ff, r)
    for (ff = f-1; ff>=0; --ff) RAY(ff, r)
    for (rr = r+1; rr<=7; ++rr) RAY(f, rr)
    for (rr = r-1; rr>=0; --rr) RAY(f, rr)
    // clang-format on

#undef RAY
    return atk;
}
static uint64_t bishop_attacks_slow(square_t sq, uint64_t occ)
{
    uint64_t atk{0};
    int r = sq.rank();
    int f = sq.file();
    int rr, ff;

#define DIAG(rcond, fcond, rstep, fstep) \
    for (rr = r+(rstep), ff = f+(fstep); rcond && fcond; rr+=(rstep), ff+=(fstep)) \
    { atk |= 1ULL << square_t{ff, rr}; if (occ & (1ULL << square_t{ff, rr})) break; }

    // clang-format off
    DIAG(rr<=7, ff<=7,  1,  1)
    DIAG(rr<=7, ff>=0,  1, -1)
    DIAG(rr>=0, ff<=7, -1,  1)
    DIAG(rr>=0, ff>=0, -1, -1)
    // clang-format on

#undef DIAG
    return atk;
}

static void find_magic(const piece_info_t& piece,
                       uint64_t (*mask_fn)(square_t),
                       uint64_t (*attacks_fn)(square_t, uint64_t),
                       magic_t* out)
{
    static constexpr int seeds[] = {728, 10316, 55013, 32803, 12281, 15100, 16645, 255};
    static uint64_t occ_list[4096];
    static uint64_t atk_list[4096];
    static uint64_t used_atk[4096];
    static uint32_t used_epoch[4096];
    std::memset(used_epoch, 0, sizeof(used_epoch));

    uint64_t mask = mask_fn(piece.sq);
    int n = bitutil::popcount(mask);
    int size = 1 << n;
    int shift = 64 - n;
    bool trying_good_magic = false;
    uint64_t magic;

    const good_magic_t& good_magic = good_magics[piece.type - piece_t::Bishop][piece.sq];

    if (good_magic.magic)
    {
        trying_good_magic = true;
        magic = good_magic.magic;
        shift = 64 - good_magic.used_bits;
    }

    {
        uint64_t sub = 0;
        for (int i = 0; i < size; ++i)
        {
            occ_list[i] = sub;
            atk_list[i] = attacks_fn(piece.sq, sub);
            sub = (sub - mask) & mask;
        }
    }

    rng::SparseRNG srng(seeds[piece.sq.rank()]);

    for (uint32_t attempt = 1;; ++attempt)
    {
        if (!trying_good_magic)
        {
            for (magic = 0; bitutil::popcount((mask * magic) >> 56) < 6;)
            {
                magic = srng.generate();
            }
        }

        bool ok = true;
        uint32_t epoch = attempt;
        if (epoch >= INT32_MAX)
        {
            std::memset(used_epoch, 0, size * sizeof(uint32_t));
            epoch = 1;
        }

        for (int i = 0; i < size; ++i)
        {
            int idx = static_cast<int>((occ_list[i] * magic) >> shift);
            if (used_epoch[idx] != epoch)
            {
                used_epoch[idx] = epoch;
                used_atk[idx] = atk_list[i];
            }
            else if (used_atk[idx] != atk_list[i])
            {
                ok = false;
                break;
            }
        }

        if (ok)
        {
            out->mask = mask;
            out->shift = shift;
            out->magic = magic;
            for (int i = 0; i < size; i++)
            {
                int idx = (int)((occ_list[i] * magic) >> shift);
                out->table[idx] = atk_list[i];
            }
            break;
        }
        else if (trying_good_magic)
        {
            std::cerr << std::hex << "Failed verifying good magic with mask: " << mask
                      << ", magic: " << magic << std::dec << std::endl;
            trying_good_magic = false;
            shift = 64 - n;
        }
    }
}

void init_magics(void)
{
    /* Assign table pointers: each square gets a contiguous slice */
    int rook_offset = 0;
    int bishop_offset = 0;

    for (int sq = 0; sq < 64; sq++)
    {
        /* --- Rook --- */
        rook_magics[sq].table = rook_table + rook_offset;
        find_magic({sq, piece_t::Rook}, rook_mask, rook_attacks_slow, &rook_magics[sq]);
        rook_offset += 1 << (64 - rook_magics[sq].shift);

        /* --- Bishop --- */
        bishop_magics[sq].table = bishop_table + bishop_offset;
        find_magic({sq, piece_t::Bishop}, bishop_mask, bishop_attacks_slow, &bishop_magics[sq]);
        bishop_offset += 1 << (64 - bishop_magics[sq].shift);
    }

    std::cout << "Rook   table entries used: " << rook_offset << std::endl;
    std::cout << "Bishop table entries used: " << bishop_offset << std::endl;
}

std::string prettyBB(uint64_t bb, const char* occ, const char* empty)
{
    std::stringstream ss;
    for (int rank = 7; rank >= 0; --rank)
    {
        ss << (rank + 1) << ' ';
        for (int file = 0; file <= 7; ++file)
        {
            square_t sq{file, rank};
            ss << ((bb >> sq) & 1 ? occ : empty) << ' ';
        }
        ss << '\n';
    }
    ss << "  a b c d e f g h\n";
    return ss.str();
}

}  // namespace chess

/* bitboard.cpp END */
