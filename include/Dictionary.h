#pragma once
#include <string>
#include <vector>
#include <unordered_set>

class Dictionary {
public:
    bool load(const std::string& path);
    bool contains(const std::string& word) const;
    const std::vector<std::string>& words() const { return words_; }
    std::size_t size() const { return words_.size(); }

private:
    std::vector<std::string> words_;
    std::unordered_set<std::string> wordSet_;
};
