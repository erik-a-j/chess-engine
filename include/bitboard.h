#ifndef BITBOARD_H
#define BITBOARD_H

#include "types.h"

namespace chess {

struct Magic {
    uint64_t* table;
    uint64_t mask;
    uint64_t magic;
    int shift;
};

extern Magic rook_magics[64];
extern Magic bishop_magics[64];
extern uint64_t rook_table[102400];
extern uint64_t bishop_table[5248];

void init_magics(void);

inline uint64_t rook_attacks(square_idx_t sq, uint64_t occ)
{
    const Magic* m = &rook_magics[sq];
    return m->table[((occ & m->mask) * m->magic) >> m->shift];
}

inline uint64_t bishop_attacks(square_idx_t sq, uint64_t occ)
{
    const Magic* m = &bishop_magics[sq];
    return m->table[((occ & m->mask) * m->magic) >> m->shift];
}

}  // namespace chess

#endif /* #ifndef BITBOARD_H */
