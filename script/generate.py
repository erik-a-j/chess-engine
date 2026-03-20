
import bitboard as bb
import decldefi
from common import GenerateInfo, DeclDefi, CType, HeaderGuard, include_generated
from doxy import doxygen

SYSTEM_INC = '#include <{}>\n'
LOCAL_INC = '#include "{}"\n'

def moves(gen: GenerateInfo):
    source_file = gen.source[0]
    header_file = gen.header[0]
    file_desc = "This file consists of precomputed arrays and generated functions\n"
    with open(source_file, mode="a", encoding="utf-8") as src, open(header_file, mode="a", encoding="utf-8") as hdr:

        dd = DeclDefi(
            decl_pp=[
                SYSTEM_INC.format("cstdint"), 
                SYSTEM_INC.format("cstddef")
            ],
            defi_pp=[
                LOCAL_INC.format(include_generated(header_file.name))
            ]
        )
        arrays: dict[str, tuple[CType, list]] = {
            "squares": (CType.uint64_t, bb.SQUARES),
            "files": (CType.uint64_t, bb.FILES),
            "ranks": (CType.uint64_t, bb.RANKS),
            "wpawn_pushes": (CType.uint64_t, [bb.leap_move(sq, [(0, 1)]) for sq in range(0, 64)]),
            "bpawn_pushes": (CType.uint64_t, [bb.leap_move(sq, [(0, -1)]) for sq in range(0, 64)]),
            "wpawn_attacks": (CType.uint64_t, [bb.leap_move(sq, bb.WPAWN_DELTAS) for sq in range(0, 64)]),
            "bpawn_attacks": (CType.uint64_t, [bb.leap_move(sq, bb.BPAWN_DELTAS) for sq in range(0, 64)]),
            "knight_attacks": (CType.uint64_t, [bb.leap_move(sq, bb.KNIGHT_DELTAS) for sq in range(0, 64)]),
            "king_attacks": (CType.uint64_t, [bb.leap_move(sq, bb.KING_DELTAS) for sq in range(0, 64)]),
            "rook_bits": (CType.uint64_t, bb.ROOK_BITS),
            "bishop_bits": (CType.uint64_t, bb.BISHOP_BITS),
        }

        for id, (ctype, values) in arrays.items():
            dd.extend(decldefi.array(id, values, ctype))
            dd.decl.code.append("\n")
        
        hguard = HeaderGuard(header_file.name)

        src.writelines(doxygen(file=source_file.name, desc=file_desc))
        hdr.writelines(doxygen(file=header_file.name, desc=file_desc))
        src.writelines(dd.defi.pp)
        src.write("\n")
        hdr.write(hguard.begin)
        hdr.writelines(dd.decl.pp)
        hdr.write("\n")
        src.writelines(dd.defi.code)
        hdr.writelines(dd.decl.code)
        hdr.write(hguard.end)

'''def bitboard(gen: GenerateInfo):
    header_file = gen.header[0]
    cmd = gen.command

    file_desc = "This file consists of generated functions\n"
    with open(header_file, mode="a", encoding="utf-8") as hdr:
        
        dd = DeclDefi(
            defi_pp=[
                SYSTEM_INC.format("cstdint"), 
                SYSTEM_INC.format("cstddef")
            ]
        )

        dd.extend(decldefi.flip_board(cmd))

        hguard = HeaderGuard(header_file.name)

        hdr.writelines(doxygen(file=header_file.name, desc=file_desc))
        hdr.write(hguard.begin)
        hdr.writelines(dd.defi.pp)
        hdr.write("\n")
        hdr.writelines(dd.defi.code)
        hdr.write("\n")
        hdr.write(hguard.end)'''