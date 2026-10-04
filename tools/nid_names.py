#!/usr/bin/env python3
"""Suggest real names for Unknown NID stubs from the public hash-verified database.

Reads every APS5_EXPORT("NID", *Unknown*) in core/, looks the NID up in the
community NID database (zecoxao/sce_symbols aerolib.csv, downloaded once into
a local cache) and prints the verified real name plus what already exists in
the tree. Every suggestion can be re-verified with --verify, which recomputes
each NID with the project's own Nid::Compute algorithm (SHA-1 + salt).

Usage:
    python tools/nid_names.py [--db PATH] [--nid NID ...] [--verify] [--json]
"""
import argparse
import hashlib
import json
import os
import re
import sys
import tempfile
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
PRX = ROOT / "core" / "libs" / "prx"
DB_URL = ("https://raw.githubusercontent.com/zecoxao/sce_symbols"
          "/main/aerolib.csv")
DEFAULT_CACHE = Path(tempfile.gettempdir()) / "anyps5-nid-db" / "aerolib.csv"

CHARSET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-"
SUFFIX = bytes([0x51, 0x8D, 0x64, 0xA6, 0x35, 0xDE, 0xD8, 0xC1,
                0xE6, 0xB0, 0x39, 0xB1, 0xC3, 0xE5, 0x52, 0x30])
ALIAS = re.compile(r'APS5_EXPORT\("([A-Za-z0-9+\-]{11})",\s*(\w*[Uu]nknown\w*)\)')


def compute_nid(symbol):
    digest = hashlib.sha1(symbol.encode() + SUFFIX).digest()
    rev = bytes(digest[7 - i] for i in range(8))
    out = []
    for i in range(0, 6, 3):
        triple = (rev[i] << 16) | (rev[i + 1] << 8) | rev[i + 2]
        out += [CHARSET[(triple >> 18) & 63], CHARSET[(triple >> 12) & 63],
                CHARSET[(triple >> 6) & 63], CHARSET[triple & 63]]
    tail = (rev[6] << 16) | (rev[7] << 8)
    out += [CHARSET[(tail >> 18) & 63], CHARSET[(tail >> 12) & 63],
            CHARSET[(tail >> 6) & 63]]
    return "".join(out)


def load_db(path):
    if not path.exists():
        path.parent.mkdir(parents=True, exist_ok=True)
        print("downloading NID database to %s ..." % path, file=sys.stderr)
        urllib.request.urlretrieve(DB_URL, str(path))
    db = {}
    with open(path, errors="replace") as handle:
        for line in handle:
            parts = line.strip().split()
            if len(parts) >= 2:
                db.setdefault(parts[0], parts[1])
    return db


def collect_unknowns():
    found = {}
    for path in sorted(PRX.rglob("*.cpp")):
        try:
            text = path.read_text(errors="replace")
        except OSError:
            continue
        for match in ALIAS.finditer(text):
            found.setdefault(match.group(1), "%s:%s" % (
                path.relative_to(ROOT), match.group(2)))
    return found


def collect_real_names():
    names = set()
    for path in sorted((ROOT / "core" / "libs").rglob("*.cpp")):
        try:
            text = path.read_text(errors="replace")
        except OSError:
            continue
        for match in re.finditer(r"\b([A-Za-z_]\w*?)_nid_postfix\b", text):
            names.add(match.group(1))
    return names


def main():
    parser = argparse.ArgumentParser(
        description="Suggest real names for Unknown NID stubs")
    parser.add_argument("--db", default=os.environ.get("ANYPS5_NID_DB"),
                        help="NID database CSV (downloaded to a cache dir by default)")
    parser.add_argument("--nid", nargs="*", default=[],
                        help="resolve only these NIDs instead of scanning the tree")
    parser.add_argument("--verify", action="store_true",
                        help="recompute every suggested NID and fail on mismatch")
    parser.add_argument("--json", action="store_true",
                        help="emit machine-readable JSON")
    args = parser.parse_args()

    db_path = Path(args.db) if args.db else DEFAULT_CACHE
    db = load_db(db_path)

    if args.nid:
        unknowns = {nid: "<cli>" for nid in args.nid}
    else:
        unknowns = collect_unknowns()
    real_names = collect_real_names()

    rows = []
    failures = 0
    for nid in sorted(unknowns):
        suggestion = db.get(nid, "")
        status = "unknown"
        if suggestion:
            if args.verify and compute_nid(suggestion) != nid:
                status = "MISMATCH"
                failures += 1
            elif suggestion in real_names:
                status = "already-implemented"
            else:
                status = "rename-ready"
        rows.append({"nid": nid, "stub": unknowns[nid],
                     "suggestion": suggestion, "status": status})

    if args.json:
        print(json.dumps({"db_entries": len(db), "rows": rows}, indent=1))
    else:
        print("%-12s %-40s %-12s %s" % ("NID", "STUB", "STATUS", "SUGGESTION"))
        for row in rows:
            print("%-12s %-40s %-12s %s" % (
                row["nid"], row["stub"][:40], row["status"], row["suggestion"]))
        print("%d unknowns, %d suggestions (%d already implemented)" % (
            len(rows), sum(1 for r in rows if r["suggestion"]),
            sum(1 for r in rows if r["status"] == "already-implemented")))
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
