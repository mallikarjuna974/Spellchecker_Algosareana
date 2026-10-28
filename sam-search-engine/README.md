# SAM Multilingual Substring Search (C++20)

A modular C++20 command-line search engine using a Suffix Automaton (SAM/DAWG), suffix links, Unicode code-point matching, document occurrence indexing, and domain-keyword ranking. Includes English and Tamil sample corpora.

## Layout
- `include/unicode.h`, `src/unicode.cpp`: strict UTF-8 decoder/encoder; ASCII case-folding only.
- `include/sam.h`, `src/sam.cpp`: suffix automaton, suffix links, substring traversal, suffix-link-tree occurrence enumeration.
- `include/index_map.h`, `src/index_map.cpp`: document metadata and direct end-position marks.
- `include/ranker.h`, `src/ranker.cpp`: deterministic relevance ranking and domain-keyword boosts.
- `include/engine.h`, `src/engine.cpp`: corpus loading and query orchestration.
- `src/main.cpp`: sample and interactive searches.

## Build and test
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```
Run from the project root with `./build/sam_search_cli`. On Windows, use `build\\Release\\sam_search_cli.exe` for Visual Studio or `build\\sam_search_cli.exe` for MinGW.

The app expects `data/domain_keywords.txt` and `data/tamil_vocabulary.txt`; `data/english_corpus.txt` is optional. Each non-empty line in the vocabulary files is treated as one document. Pass an alternate data directory as the first CLI argument.

## Complexity
For N indexed code points, SAM construction is O(N) amortized with expected O(1) hash-map transitions. Query traversal is O(m) expected for query length m. Occurrence enumeration traverses a suffix-link subtree and can inspect O(states + returned occurrences) nodes. This reference ranker scans each matched document for each keyword; optimize with a keyword/document index for large corpora.

## Unicode caveats
UTF-8 is strictly validated. Search operates on Unicode code points, not grapheme clusters. Only ASCII case-folding is performed; no NFC/NFD normalization or locale-aware Unicode case folding is implemented. Canonically equivalent Tamil sequences may therefore compare differently. Consider ICU for canonical normalization and grapheme-aware behavior.

## Scope
This is a tested starter architecture, not production certification. Benchmark memory and throughput on representative corpora before deployment. The SAM uses an internal sentinel outside the Unicode scalar range between documents so ordinary UTF-8 queries cannot match across document boundaries.
