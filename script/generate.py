
import bitboard as bb
import decldefi
from common import GenerateInfo, DeclDefi, CType, HeaderGuard, include_generated
from doxy import doxygen

SYSTEM_INC = '#include <{}>\n'
LOCAL_INC = '#include "{}"\n'

def precomputed(gen: GenerateInfo):
    header_file = gen.header[0]
    file_desc = "This file consists of precomputed arrays and generated functions\n"

    with open(header_file, mode="a", encoding="utf-8") as hdr:
        dd = DeclDefi(
            defi_pp=[
                SYSTEM_INC.format("cstdint"), 
                SYSTEM_INC.format("cstddef")
            ]
        )
        arrays: dict[str, tuple[CType, list]] = {
            "BBsquares": (CType.uint64_t, bb.SQUARES),
            "BBfiles": (CType.uint64_t, bb.FILES),
            "BBranks": (CType.uint64_t, bb.RANKS),
            "BBwpawn_pushes": (CType.uint64_t, [bb.leap_move(sq, [(0, 1)]) for sq in range(0, 64)]),
            "BBbpawn_pushes": (CType.uint64_t, [bb.leap_move(sq, [(0, -1)]) for sq in range(0, 64)]),
            "BBwpawn_attacks": (CType.uint64_t, [bb.leap_move(sq, bb.WPAWN_DELTAS) for sq in range(0, 64)]),
            "BBbpawn_attacks": (CType.uint64_t, [bb.leap_move(sq, bb.BPAWN_DELTAS) for sq in range(0, 64)]),
            "BBknight_attacks": (CType.uint64_t, [bb.leap_move(sq, bb.KNIGHT_DELTAS) for sq in range(0, 64)]),
            "BBking_attacks": (CType.uint64_t, [bb.leap_move(sq, bb.KING_DELTAS) for sq in range(0, 64)]),
            "BBrook_bits": (CType.uint64_t, bb.ROOK_BITS),
            "BBbishop_bits": (CType.uint64_t, bb.BISHOP_BITS),
        }

        for id, (ctype, values) in arrays.items():
            dd.extend(decldefi.array(id, values, ctype, defi_only=True))
            dd.defi.code.append("\n")
        
        hguard = HeaderGuard(header_file.name)

        hdr.writelines(doxygen(file=header_file.name, desc=file_desc))
        hdr.write(hguard.begin)
        hdr.writelines(dd.defi.pp)
        hdr.write("\n")
        hdr.writelines(dd.defi.code)
        hdr.write(hguard.end)
