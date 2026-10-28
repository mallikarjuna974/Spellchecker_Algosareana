#include "Dictionary.h"
#include <algorithm>
#include <cctype>
#include <fstream>

namespace {
std::string normalize(std::string s) {
    std::string out;
    out.reserve(s.size());
    for (unsigned char c : s) {
        if (std::isalpha(c) || std::isdigit(c)) out.push_back(static_cast<char>(std::tolower(c)));
    }
    return out;
}
}

bool Dictionary::load(const std::string& path) {
    std::ifstream in(path);
    if (!in) return false;

    words_.clear();
    wordSet_.clear();
    std::string line;
    while (std::getline(in, line)) {
        std::string word = normalize(line);
        if (!word.empty() && wordSet_.insert(word).second) words_.push_back(word);
    }
    std::sort(words_.begin(), words_.end());
    return !words_.empty();
}

bool Dictionary::contains(const std::string& word) const {
    return wordSet_.find(normalize(word)) != wordSet_.end();
}
