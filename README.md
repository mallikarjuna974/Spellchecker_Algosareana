# Spellchecker & Multilingual Autocorrect (C++17)

Two command-line programs: the original English substring autocorrect, plus a Unicode-aware Indic spellchecker for **Telugu, Tamil, and Hindi**.

## Multilingual features
- User chooses Telugu (తెలుగు), Tamil (தமிழ்), or Hindi (हिन्दी) at launch.
- UTF-8 decoding/encoding and edit-distance comparisons operate on Unicode code points, not raw UTF-8 bytes.
- Separate dictionary file per language: `data/te.txt`, `data/ta.txt`, `data/hi.txt`.
- Ranked suggestions with edit distance, substring matching, and an interactive accept/keep choice.
- Script validation prevents accidentally searching a dictionary using a different Indic script.
- No network request is made by the app itself.

## Dictionary setup (10,000 unique words per language)
The 30,000-word data files are obtained from the Indic Project dictionaries (GPLv2; see upstream attribution and license). They are not fabricated placeholders and are intentionally not committed as generated files.

```bash
python scripts/fetch_dictionaries.py
```

The script filters the Telugu, Tamil, and Hindi upstream lists by Unicode script and writes exactly 10,000 unique words into each data file. It fails instead of silently padding a short list. Internet access is needed for this one-time setup. Review the upstream license before redistributing dictionary data.

Upstream: https://gitlab.com/indicproject/dictionaries (GPLv2).

## Build
```bash
cmake -S . -B build
cmake --build build
``

Run the original English program:
```bash
./build/autocorrect
``

Run the multilingual program after dictionary setup:
```bash
./build/multilingual_autocorrect
``

Windows PowerShell:
```powershell
cmake -S . -B build
cmake --build build --config Release
python scripts/fetch_dictionaries.py
.\build\Release\multilingual_autocorrect.exe
``

At the prompt, type a word in the selected script. Enter `:quit` to exit.

## Algorithm and limitations
Unicode edit distance uses code points and adjacent transposition support. Candidate ranking is currently a simple baseline: edit distance, substring match, and length difference. This is not full grapheme-cluster-aware normalization; combining marks can count as separate code points, and language-specific morphology/context are not modeled. For a production spellchecker, Unicode normalization (NFC), grapheme-aware distance, word-frequency tuning, and native-speaker evaluation are recommended.

## Original English version
The original English implementation and its deterministic 10,000-entry vocabulary remain available as `autocorrect`.

## Tests
```bash
bash tests/smoke_test.sh
``
