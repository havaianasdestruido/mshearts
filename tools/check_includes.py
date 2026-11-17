#!/usr/bin/env python3
"""Verify that every quoted #include ("...") in the source tree resolves to
a file that exists in the repository.

This is the automatable part of issue #9 ("Repository is missing multiple
source headers and any build system -- cannot compile").  A full compile is
not possible in this sandbox (no Windows/MSVC/MFC), but we can at least
guarantee that no include refers to a missing file.
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "mshearts"

INCLUDE_RE = re.compile(r'^\s*#\s*include\s+"([^"]+)"')


def check_includes():
    missing = []
    checked = 0
    for path in sorted(SRC.rglob("*")):
        if path.suffix.lower() not in (".cpp", ".h", ".c", ".rc", ".inl"):
            continue
        try:
            text = path.read_text(errors="replace")
        except OSError:
            continue
        for lineno, line in enumerate(text.splitlines(), 1):
            m = INCLUDE_RE.match(line)
            if not m:
                continue
            checked += 1
            inc = m.group(1)
            # Standard C/C++ library headers come with the toolchain, not the
            # repo (stdlib.h, string.h, ...).  Issue #9 only concerns
            # project-local headers.
            SYSTEM_HEADERS = {
                "stdlib.h", "string.h", "stdio.h", "math.h", "tchar.h",
                "cmath", "cstdlib", "cstring",
            }
            if inc in SYSTEM_HEADERS:
                continue
            candidates = [path.parent / inc, ROOT / inc]
            if not any(c.exists() for c in candidates):
                missing.append(f"{path.relative_to(ROOT)}:{lineno}: {inc}")
    return checked, missing


def main():
    checked, missing = check_includes()
    print(f"checked {checked} quoted #include directives")
    if missing:
        print("MISSING INCLUDES:")
        for item in missing:
            print(f"  {item}")
        sys.exit(1)
    print("all quoted includes resolve to files in the repository")


if __name__ == "__main__":
    main()
