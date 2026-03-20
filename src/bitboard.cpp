/* bitboard.cpp BEGIN */

#include <cstring>
#include <iostream>

#include "macro.h"

#include "bitboard.h"
#include "bitutil.h"
#include "rng.h"

namespace chess {

Magic rook_magics[64];
Magic bishop_magics[64];
uint64_t rook_table[102400];
uint64_t bishop_table[5248];

static uint64_t rook_mask(square_idx_t sq)
{
    uint64_t mask{0};
    int r = BBrank(sq);
    int f = BBfile(sq);

    // clang-format off
    for (int ff = f+1; ff<=6; ++ff) mask |= 1ULL << BBsq(ff, r);
    for (int ff = f-1; ff>=1; --ff) mask |= 1ULL << BBsq(ff, r);
    for (int rr = r+1; rr<=6; ++rr) mask |= 1ULL << BBsq(f, rr);
    for (int rr = r-1; rr>=1; --rr) mask |= 1ULL << BBsq(f, rr);
    // clang-format on

    return mask;
}
static uint64_t bishop_mask(square_idx_t sq)
{
    uint64_t mask{0};
    int r = BBrank(sq);
    int f = BBfile(sq);
    int rr, ff;

    // clang-format off
    for (rr = r+1, ff = f+1; rr<=6 && ff<=6; ++rr, ++ff) mask |= 1ULL << BBsq(ff, rr);
    for (rr = r+1, ff = f-1; rr<=6 && ff>=1; ++rr, --ff) mask |= 1ULL << BBsq(ff, rr);
    for (rr = r-1, ff = f+1; rr>=1 && ff<=6; --rr, ++ff) mask |= 1ULL << BBsq(ff, rr);
    for (rr = r-1, ff = f-1; rr>=1 && ff>=1; --rr, --ff) mask |= 1ULL << BBsq(ff, rr);
    // clang-format on

    return mask;
}
static uint64_t rook_attacks_slow(square_idx_t sq, uint64_t occ)
{
    uint64_t atk{0};
    int r = BBrank(sq);
    int f = BBfile(sq);
    int rr, ff;

#define RAY(file, rank) \
    { atk |= 1ULL << BBsq((file), (rank)); \
      if (occ & (1ULL << BBsq((file), (rank)))) break; }

    // clang-format off
    for (ff = f+1; ff<=7; ++ff) RAY(ff, r)
    for (ff = f-1; ff>=0; --ff) RAY(ff, r)
    for (rr = r+1; rr<=7; ++rr) RAY(f, rr)
    for (rr = r-1; rr>=0; --rr) RAY(f, rr)
    // clang-format on

#undef RAY
    return atk;
}
static uint64_t bishop_attacks_slow(square_idx_t sq, uint64_t occ)
{
    uint64_t atk{0};
    int r = BBrank(sq);
    int f = BBfile(sq);
    int rr, ff;

#define DIAG(rcond, fcond, rstep, fstep) \
    for (rr = r+(rstep), ff = f+(fstep); rcond && fcond; rr+=(rstep), ff+=(fstep)) \
    { atk |= 1ULL << BBsq(ff, rr); if (occ & (1ULL << BBsq(ff, rr))) break; }

    // clang-format off
    DIAG(rr<=7, ff<=7,  1,  1)
    DIAG(rr<=7, ff>=0,  1, -1)
    DIAG(rr>=0, ff<=7, -1,  1)
    DIAG(rr>=0, ff>=0, -1, -1)
    // clang-format on

#undef DIAG
    return atk;
}

static void find_magic(square_idx_t sq,
                       uint64_t mask,
                       uint64_t (*attacks_fn)(square_idx_t, uint64_t),
                       Magic* out)
{
    int n = bitutil::popcount(mask);
    int size = 1 << n;
    int shift = 64 - n;

    uint64_t* occ_list = new uint64_t[size];
    uint64_t* atk_list = new uint64_t[size];

    {
        uint64_t sub = 0;
        for (int i = 0; i < size; ++i)
        {
            occ_list[i] = sub;
            atk_list[i] = attacks_fn(sq, sub);
            sub = (sub - mask) & mask;
        }
    }

    uint64_t* used_atk = new uint64_t[size];
    int* used_epoch = new int[size];
    std::memset(used_epoch, -1, size * sizeof(int));

    rng::SparseRNG srng(54321);
    uint64_t magic;
    for (int attempt = 0;; ++attempt)
    {
        for (magic = 0; bitutil::popcount((mask * magic) >> 56) < 6;)
        {
            magic = srng.generate();
        }

        bool ok = true;
        int epoch = attempt;

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
    }

    delete[] occ_list;
    delete[] atk_list;
    delete[] used_atk;
    delete[] used_epoch;
}

void init_magics(void)
{
    /* Assign table pointers: each square gets a contiguous slice */
    int rook_offset = 0;
    int bishop_offset = 0;

    for (int sq = 0; sq < 64; sq++)
    {
        /* --- Rook --- */
        uint64_t rmask = rook_mask(sq);
        rook_magics[sq].table = rook_table + rook_offset;
        find_magic(sq, rmask, rook_attacks_slow, &rook_magics[sq]);
        rook_offset += 1 << bitutil::popcount(rmask);

        /* --- Bishop --- */
        uint64_t bmask = bishop_mask(sq);
        bishop_magics[sq].table = bishop_table + bishop_offset;
        find_magic(sq, bmask, bishop_attacks_slow, &bishop_magics[sq]);
        bishop_offset += 1 << bitutil::popcount(bmask);
    }

    std::cout << "Rook   table entries used: " << rook_offset << std::endl;
    std::cout << "Bishop table entries used: " << bishop_offset << std::endl;
}

}  // namespace chess

/* bitboard.cpp END */
