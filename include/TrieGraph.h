#pragma once
#include <string>
#include <unordered_map>
#include <vector>

// A trie is a directed, character-labelled graph: each node represents a prefix.
class TrieGraph {
public:
    struct Node {
        std::unordered_map<char, int> edges;
        bool terminal = false;
        std::string word;
    };

    void build(const std::vector<std::string>& words);
    bool contains(const std::string& word) const;
    std::size_t nodeCount() const { return nodes_.size(); }
    std::vector<std::string> wordsWithPrefix(const std::string& prefix,
                                             std::size_t limit = 100) const;

private:
    std::vector<Node> nodes_;
    void collect(int node, std::vector<std::string>& out, std::size_t limit) const;
};
