#ifndef BITBOARD_H
#define BITBOARD_H

#include <string>
#include "constant.h"
#include "types.h"

namespace chess {

constexpr size_t bishop_table_SIZE = k::rb_table_size(piece_t::Bishop);
constexpr size_t rook_table_SIZE = k::rb_table_size(piece_t::Rook);
extern uint64_t bishop_table[bishop_table_SIZE];
extern uint64_t rook_table[rook_table_SIZE];
extern magic_t bishop_magics[64];
extern magic_t rook_magics[64];

void init_magics(void);
std::string prettyBB(uint64_t bb, const char* occ = "■", const char* empty = "•");

}  // namespace chess

#endif /* #ifndef BITBOARD_H */
