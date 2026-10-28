# Spellchecker / Substring Autocorrect (C++17)

A modular command-line autocorrect baseline with a deterministic 10,000-entry vocabulary containing common English and computer-science terms.

## Modules

1. **Dictionary** — loads, normalizes, and deduplicates entries.
2. **Trie Graph** — a directed character-labelled graph whose root-to-terminal paths represent words.
3. **Candidate Generation** — checks one-edit variants: insertion, deletion, substitution, and adjacent transposition.
4. **Ranking** — sorts candidates by Levenshtein edit distance, then a length-aware score.
5. **CLI / Integration** — loads the vocabulary, checks input, and prints ranked suggestions.

## Requirements

- C++17 compiler
- CMake 3.16+
- Python 3 (generates the vocabulary at configure time)

## Build and run

```bash
cmake -S . -B build
cmake --build build
./build/autocorrect
```

On Windows with Visual Studio:

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\autocorrect.exe
```

Run from the repository root, or pass an explicit vocabulary path:

```bash
./build/autocorrect data/dictionary.txt
```

## Example

```text
Word> algoritm
Unknown word: algoritm
Top suggestions:
  1. algorithm [edit distance: 1, score: 84]
Autocorrect: algorithm
```

## Tests

```bash
bash tests/smoke_test.sh
```

## Limitations

This is a baseline, not a context-aware language model. The deterministic vocabulary includes generated compounds and fallback alphabetic entries to reach exactly 10,000 unique terms; some entries are not standard dictionary headwords. Ranking is based on spelling similarity, not sentence context. Normalization is ASCII-oriented, and multi-error misspellings may not be corrected.
