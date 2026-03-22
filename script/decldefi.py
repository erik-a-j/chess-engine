from common import CType, PpAndCode, DeclDefi
from doxy import doxygen

def array(
        array_id: str,
        values: list,
        value_type: CType,
        linebreak: int = 4,
        indent: int = 0,
        defi_only: bool = False) -> DeclDefi:

    t = " " * indent
    array_length = len(values)
    array_length_id = f"{array_id}_SIZE"
    prototype = f"{value_type.typeId} {array_id}[{array_length_id}]"

    array_brief = f"`{array_length_id}` array-length of `{array_id}`"
    
    decl = PpAndCode()
    defi = PpAndCode()

    array_doc = doxygen(brief=array_brief, indent=indent)
    size_decl = f"{t}constexpr std::size_t {array_length_id} = {array_length}ULL;\n"

    if not defi_only:
        decl.code = array_doc + [
            size_decl,
            f"{t}extern const {prototype};\n"
        ]
        defi.code = [
            f"{t}const {prototype} = {{\n", 
            f"{t}}};\n"
        ]
    else:
        defi.code = array_doc + [
            size_decl,
            f"{t}constexpr {prototype} = {{\n", 
            f"{t}}};\n"
        ]

    for i in range(0, array_length, linebreak):
        line = ", ".join(value_type.literal(v) for v in values[i:i+linebreak])
        defi.code.insert(-1, f"{t}\t{line},\n")

    return DeclDefi(decl, defi)
