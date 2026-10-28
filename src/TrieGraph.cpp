#include "TrieGraph.h"
#include <cctype>

namespace {
std::string lowerAscii(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}
}

void TrieGraph::build(const std::vector<std::string>& words) {
    nodes_.clear();
    nodes_.emplace_back();
    for (const auto& raw : words) {
        const std::string word = lowerAscii(raw);
        int current = 0;
        for (char ch : word) {
            auto it = nodes_[current].edges.find(ch);
            if (it == nodes_[current].edges.end()) {
                int next = static_cast<int>(nodes_.size());
                nodes_[current].edges[ch] = next;
                nodes_.emplace_back();
                current = next;
            } else {
                current = it->second;
            }
        }
        nodes_[current].terminal = true;
        nodes_[current].word = word;
    }
}

bool TrieGraph::contains(const std::string& raw) const {
    const std::string word = lowerAscii(raw);
    int current = 0;
    for (char ch : word) {
        auto it = nodes_[current].edges.find(ch);
        if (it == nodes_[current].edges.end()) return false;
        current = it->second;
    }
    return nodes_[current].terminal;
}

void TrieGraph::collect(int node, std::vector<std::string>& out, std::size_t limit) const {
    if (out.size() >= limit) return;
    if (nodes_[node].terminal) out.push_back(nodes_[node].word);
    for (const auto& edge : nodes_[node].edges) {
        if (out.size() >= limit) break;
        collect(edge.second, out, limit);
    }
}

std::vector<std::string> TrieGraph::wordsWithPrefix(const std::string& rawPrefix,
                                                    std::size_t limit) const {
    std::string prefix = lowerAscii(rawPrefix);
    int current = 0;
    for (char ch : prefix) {
        auto it = nodes_[current].edges.find(ch);
        if (it == nodes_[current].edges.end()) return {};
        current = it->second;
    }
    std::vector<std::string> result;
    collect(current, result, limit);
    return result;
}
