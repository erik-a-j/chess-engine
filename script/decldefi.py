from common import CType, PpAndCode, DeclDefi
from doxy import doxygen

def array(
        array_id: str,
        values: list,
        value_type: CType,
        linebreak: int = 4,
        indent: int = 0) -> DeclDefi:

    t = " " * indent
    array_length = len(values)
    array_length_id = f"{array_id}_SIZE"
    prototype = f"const {value_type.typeId} {array_id}[{array_length_id}]"

    array_desc = f"@c {array_length_id} array-length of @c {id}\n@c {array_id} array"
    
    decl = PpAndCode(code=doxygen(desc=array_desc, indent=indent) + [
        f"{t}constexpr std::size_t {array_length_id} = {array_length}ULL;\n",
        f"{t}extern {prototype};\n"
    ])
    defi = PpAndCode(code=[f"{t}{prototype} {{\n", f"{t}}};\n"])

    for i in range(0, array_length, linebreak):
        line = ", ".join(value_type.literal(v) for v in values[i:i+linebreak])
        defi.code.insert(-1, f"{t}\t{line},\n")

    return DeclDefi(decl, defi)

'''def flip_board(cmd: list[str]) -> DeclDefi:
    std_v = None
    std = "c++11"
    for arg in cmd:
        if arg.startswith("-std="):
            std = arg[5:]
            break

    for i in range(0, len(std)):
        if std[i:].isnumeric():
            std_v = int(std[i:])
            break

    if std_v is None:
        raise ValueError("could not parse std-version from param 'std'")
    
    param_id = "bb"
    prototype = f"std::uint64_t flip_board(std::uint64_t {param_id}) noexcept"

    defi = PpAndCode(code=doxygen(brief="Returns a vertically flipped bitboard", params=[(param_id, "Bitboard to flip")]))
    defi.code.extend([f"{prototype} {{\n", "}\n"])

    if std_v >= 23:
        defi.pp.append("#include <bit>\n")
        defi.code.insert(-1, f"\treturn std::byteswap({param_id});\n")
    else:
        defi.code.insert(-1, f"\treturn __builtin_bswap64({param_id});\n")
    
    return DeclDefi(PpAndCode(), defi)
'''