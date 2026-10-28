#!/usr/bin/env bash
set -euo pipefail
python3 scripts/fetch_dictionaries.py
python3 - <<'PY'
from pathlib import Path
for code in ("te","ta","hi"):
    p=Path("data")/f"{code}.txt"
    words=p.read_text(encoding="utf-8").splitlines()
    assert len(words)==10000, (code,len(words))
    assert len(set(words))==10000, code
    assert all(words), code
    print(f"{code}: {len(words)} unique entries")
PY
printf '1\n:quit\n' | ./build/multilingual_autocorrect | grep -q 'Telugu'
printf '2\n:quit\n' | ./build/multilingual_autocorrect | grep -q 'Tamil'
printf '3\n:quit\n' | ./build/multilingual_autocorrect | grep -q 'Hindi'
echo "Multilingual CLI smoke tests passed"
