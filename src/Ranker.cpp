#include "Ranker.h"
#include <algorithm>
#include <cmath>
#include <unordered_set>

int Ranker::levenshtein(const std::string& a, const std::string& b) {
    std::vector<int> previous(b.size() + 1), current(b.size() + 1);
    for (std::size_t j = 0; j <= b.size(); ++j) previous[j] = static_cast<int>(j);
    for (std::size_t i = 1; i <= a.size(); ++i) {
        current[0] = static_cast<int>(i);
        for (std::size_t j = 1; j <= b.size(); ++j) {
            int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
            current[j] = std::min({
                previous[j] + 1,
                current[j - 1] + 1,
                previous[j - 1] + cost
            });
        }
        previous.swap(current);
    }
    return previous[b.size()];
}

std::vector<Suggestion> Ranker::rank(const std::string& input,
                                     const std::vector<std::string>& candidates,
                                     std::size_t limit) const {
    std::vector<Suggestion> results;
    std::unordered_set<std::string> seen;
    for (const auto& candidate : candidates) {
        if (!seen.insert(candidate).second) continue;
        int distance = levenshtein(input, candidate);
        double score = 100.0 - distance * 15.0
                     - std::abs(static_cast<double>(input.size()) -
                                static_cast<double>(candidate.size())) * 0.5;
        results.push_back({candidate, distance, score});
    }
    std::sort(results.begin(), results.end(), [](const Suggestion& a, const Suggestion& b) {
        if (a.editDistance != b.editDistance) return a.editDistance < b.editDistance;
        if (a.score != b.score) return a.score > b.score;
        return a.word < b.word;
    });
    if (results.size() > limit) results.resize(limit);
    return results;
}
