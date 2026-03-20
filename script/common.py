from typing import NamedTuple, overload
from dataclasses import dataclass, field
from pathlib import Path
from enum import Enum

class FileId(Enum):
    moves = 0
    bitboard = 1

class GenerateInfo(NamedTuple):
    source: list[Path]
    header: list[Path]
    command: list[str]

@dataclass
class PpAndCode:
    pp: list[str]  = field(default_factory=list)
    code: list[str] = field(default_factory=list)

@dataclass
class DeclDefi:
    decl: PpAndCode = field(default_factory=PpAndCode)
    defi: PpAndCode = field(default_factory=PpAndCode)

    def __init__(self, decl: PpAndCode = None, defi: PpAndCode = None, *, 
                 decl_pp: list[str] = None, decl_code: list[str] = None, 
                 defi_pp: list[str] = None, defi_code: list[str] = None):
        if decl is not None:
            self.decl = decl
        else:
            self.decl = PpAndCode(pp=decl_pp or [], code=decl_code or [])
        if defi is not None:
            self.defi = defi
        else:
            self.defi = PpAndCode(pp=defi_pp or [], code=defi_code or [])
        

    def extend(self, other: "DeclDefi"):
        self.decl.pp.extend(other.decl.pp)
        self.decl.code.extend(other.decl.code)
        self.defi.pp.extend(other.defi.pp)
        self.defi.code.extend(other.defi.code)


_GEN_DIR_STEM = "generated"
def include_generated(name: str) -> str:
    return f"{_GEN_DIR_STEM}/{name}"

class HeaderGuard:
    def __init__(self, name: str):
        self._id = f"{_GEN_DIR_STEM}_{name.replace('-', '_').replace('.', '_')}".upper()
    
    @property
    def id(self) -> str:
        return self._id
    @property
    def begin(self) -> str:
        return f"#ifndef {self._id}\n#define {self._id}\n\n"
    @property
    def end(self) -> str:
        return f"#endif //{self._id}\n"

class _T(Enum):
    S = 0
    U = 1
    STR = 2
class CType(Enum):
    int8_t     = (_T.S,   8,  "")
    int16_t    = (_T.S,   16, "")
    int32_t    = (_T.S,   32, "")
    int64_t    = (_T.S,   64, "LL")
    uint8_t    = (_T.U,   8,  "U")
    uint16_t   = (_T.U,   16, "U")
    uint32_t   = (_T.U,   32, "U")
    uint64_t   = (_T.U,   64, "ULL")
    strliteral = (_T.STR, 64, "")

    def __new__(cls, t: _T, width: int, suffix: str) -> "CType":
        obj = object.__new__(cls)
        obj._value_ = (t, width, suffix)
        return obj

    @property
    def T(self) -> type:
        if self.value[0] == _T.STR:
            return str
        return int
    
    @property
    def signed(self) -> bool:
        return self.value[0] == _T.S
    @property
    def width(self) -> int:
        return self.value[1]
    @property
    def suffix(self) -> str:
        return self.value[2]
    @property
    def typeId(self) -> str:
        return f"std::{self.name}" if self.T == int else "char*"
    @property
    def mask(self) -> int:
        return (1 << self.width) - 1
    
    def __str__(self) -> str:
        return self.typeId

    def literal(self, value: int) -> str:
        if self.T == str:
            return f'"{value}"'
        v = value & self.mask
        return f"0x{v:0{self.width // 4}X}{self.suffix}"

def init_files(files: list[Path]) -> bool:
    for f in files:
        try:
            Path.mkdir(f.parent, parents=True, exist_ok=True)
            with open(f, "w"):
                pass
        except Exception as e:
            print("Error:", e)
            return False
    return True

def print_hex(x):
    if isinstance(x, list):
        print(list(map(hex, x)))
    else:
        print(hex(x))

__all__ = [
    "GenerateInfo", 
    "PpAndCode", 
    "DeclDefi", 
    "include_generated",
    "HeaderGuard", 
    "FileId", 
    "CType", 
    "init_files", 
    "print_hex"
]
