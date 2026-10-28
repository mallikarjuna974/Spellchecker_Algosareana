#pragma once
#include <string>
#include <vector>

struct Suggestion {
    std::string word;
    int editDistance = 0;
    double score = 0.0;
};

class Ranker {
public:
    std::vector<Suggestion> rank(const std::string& input,
                                 const std::vector<std::string>& candidates,
                                 std::size_t limit = 5) const;
    static int levenshtein(const std::string& a, const std::string& b);
};
