
from pathlib import Path
import sys

import shlex
import json

import generate
from common import *

def main(gen: dict[FileId, GenerateInfo]):
    all_files = []
    for g in gen.values():
        all_files.extend(g.source)
        all_files.extend(g.header)
    if not init_files(all_files):
        exit(1)

    generate.moves(gen[FileId.moves])
    #generate.bitboard(gen[FileId.bitboard])


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Expected args for path to manifest and compile_commands")
        exit(1)
    try:
        with open(sys.argv[1], "r", encoding="utf-8") as manifest, open(sys.argv[2], "r", encoding="utf-8") as compile_commands:
            entries = manifest.readlines()
            ccmd: list[dict] = json.load(compile_commands)
    except Exception as e:
        print("Error:", e)
        exit(1)
    
    global_cflags = sys.argv[3:]
    generated: dict[FileId, GenerateInfo] = dict()
    for ent in entries:
        ent = ent.split("|")
        fid = FileId.__members__.get(ent[0])
        if fid is None:
            print("Invalid filename id:", ent[0])
            exit(1)

        files = [f.strip() for f in ent[1].split(' ')]
        source = [src for src in files if src.endswith(".cpp")]
        header = [hdr for hdr in files if hdr.endswith(".h")]
        command = None

        for d in ccmd:
            for f in files:
                if d["file"] == f:
                    command = shlex.split(d["command"])

        if command is None:
            command = global_cflags

        generated[fid] = GenerateInfo(
            source=[Path(p) for p in source],
            header=[Path(p) for p in header],
            command=command
        )

    main(generated)