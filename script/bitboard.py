
def U64(v: int) -> int:
    return v & ((1 << 64) - 1)

SQUARES = [U64(1 << i) for i in range(0, 64)]
FILES = [U64(0x0101010101010101 << i) for i in range(0, 8)]
RANKS = [U64(0xff << i) for i in range(0, 64, 8)]

FRAME = FILES[0] | RANKS[0] | FILES[7] | RANKS[7]

ROOK_BITS = [
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
]
BISHOP_BITS = [
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
]
KNIGHT_DELTAS = [
    (-2, -1), (-2, 1), (-1, -2), (-1, 2),
    (1, -2), (1, 2), (2, -1), (2, 1),
]
KING_DELTAS = [
    (-1, -1), (-1, 0), (-1, 1), (0, 1), 
    (1, 1), (1, 0), (1, -1), (0, -1),
]
WPAWN_DELTAS = [
    (-1, 1), (1, 1)
]
BPAWN_DELTAS = [
    (1, -1), (-1, -1)
]

def Sq(file: int, rank: int) -> int:
    return rank * 8 + file

def File(sq: int) -> int:
    return sq & 7
def Rank(sq: int) -> int:
    return sq >> 3

def Diagonal(sq: int) -> int:
    return File(sq) - Rank(sq) + 7
def Adiagonal(sq: int) -> int:
    return File(sq) + Rank(sq)

def leap_move(sq: int, deltas: list[tuple[int, int]]) -> int:
    bb = 0
    file, rank = File(sq), Rank(sq)
    for fD, rD in deltas:
        f = file + fD
        r = rank + rD
        if 0 <= f < 8 and 0 <= r < 8:
            bb |= U64(1 << Sq(f, r))
    return bb

