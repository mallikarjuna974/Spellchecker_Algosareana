#!/usr/bin/env bash
set -euo pipefail
cmake -S . -B build
cmake --build build
output="$(printf "algoritm\nrecieve\nexit\n" | ./build/autocorrect)"
grep -q "Autocorrect: algorithm" <<<"$output"
grep -q "Autocorrect: receive" <<<"$output"
echo "Smoke tests passed."
