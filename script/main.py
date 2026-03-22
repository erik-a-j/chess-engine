
from pathlib import Path
import sys

import generate
from common import *

def main(gen: dict[FileId, GenerateInfo]):
    all_files = []
    for g in gen.values():
        all_files.extend(g.source)
        all_files.extend(g.header)
    if not init_files(all_files):
        exit(1)

    generate.precomputed(gen[FileId.precomputed])


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Expected args for path to manifest")
        exit(1)
    try:
        with open(sys.argv[1], "r", encoding="utf-8") as manifest:
            entries = manifest.readlines()
    except Exception as e:
        print("Error:", e)
        exit(1)
    
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

        generated[fid] = GenerateInfo(
            source=[Path(p) for p in source],
            header=[Path(p) for p in header]
        )

    main(generated)