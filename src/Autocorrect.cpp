#include "Autocorrect.h"
#include <algorithm>
#include <cctype>
#include <unordered_set>

std::string Autocorrect::normalize(const std::string& word) {
    std::string out;
    out.reserve(word.size());
    for (unsigned char c : word) {
        if (std::isalnum(c)) out.push_back(static_cast<char>(std::tolower(c)));
    }
    return out;
}

bool Autocorrect::initialize(const std::string& dictionaryPath) {
    if (!dictionary_.load(dictionaryPath)) return false;
    trie_.build(dictionary_.words());
    return true;
}

bool Autocorrect::isCorrect(const std::string& word) const {
    return trie_.contains(normalize(word));
}

std::vector<std::string> Autocorrect::candidateWords(const std::string& rawWord) const {
    const std::string word = normalize(rawWord);
    std::unordered_set<std::string> candidates;
    if (!word.empty()) {
        auto prefixed = trie_.wordsWithPrefix(word.substr(0, 1), 250);
        for (const auto& candidate : prefixed) candidates.insert(candidate);
    }
    for (std::size_t i = 0; i < word.size(); ++i) {
        std::string variant = word.substr(0, i) + word.substr(i + 1);
        if (!variant.empty() && trie_.contains(variant)) candidates.insert(variant);
    }
    for (std::size_t i = 0; i <= word.size(); ++i) {
        for (char c = 'a'; c <= 'z'; ++c) {
            std::string variant = word.substr(0, i) + c + word.substr(i);
            if (trie_.contains(variant)) candidates.insert(variant);
        }
    }
    for (std::size_t i = 0; i < word.size(); ++i) {
        std::string variant = word;
        for (char c = 'a'; c <= 'z'; ++c) {
            if (c == word[i]) continue;
            variant[i] = c;
            if (trie_.contains(variant)) candidates.insert(variant);
        }
    }
    for (std::size_t i = 0; i + 1 < word.size(); ++i) {
        std::string variant = word;
        std::swap(variant[i], variant[i + 1]);
        if (trie_.contains(variant)) candidates.insert(variant);
    }
    if (candidates.empty() && !word.empty()) {
        auto prefixed = trie_.wordsWithPrefix(word.substr(0, 1), 250);
        for (const auto& candidate : prefixed) candidates.insert(candidate);
    }
    return {candidates.begin(), candidates.end()};
}

std::vector<Suggestion> Autocorrect::suggest(const std::string& rawWord, std::size_t limit) const {
    const std::string word = normalize(rawWord);
    if (word.empty()) return {};
    auto candidates = candidateWords(word);
    return ranker_.rank(word, candidates, limit);
}
