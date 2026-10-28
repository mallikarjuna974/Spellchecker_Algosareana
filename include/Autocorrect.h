#pragma once
#include "Dictionary.h"
#include "Ranker.h"
#include "TrieGraph.h"
#include <string>
#include <vector>

class Autocorrect {
public:
    bool initialize(const std::string& dictionaryPath);
    bool isCorrect(const std::string& word) const;
    std::vector<Suggestion> suggest(const std::string& word, std::size_t limit = 5) const;

    std::size_t dictionarySize() const { return dictionary_.size(); }
    std::size_t graphNodeCount() const { return trie_.nodeCount(); }

private:
    Dictionary dictionary_;
    TrieGraph trie_;
    Ranker ranker_;

    static std::string normalize(const std::string& word);
    std::vector<std::string> candidateWords(const std::string& word) const;
};
