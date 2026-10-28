#include "MultiDictionary.h"
#include "Unicode.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <vector>

namespace spell {
int editDistanceUnicode(const std::string& a, const std::string& b) {
    const auto x = decodeUtf8(a);
    const auto y = decodeUtf8(b);
    if (x.size() > y.size() + 3 || y.size() > x.size() + 3) return 4;

    // Optimal-string-alignment distance: insertion, deletion, substitution,
    // and one adjacent transposition per substring.
    const std::size_t m = x.size(), n = y.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
    for (std::size_t i = 0; i <= m; ++i) dp[i][0] = static_cast<int>(i);
    for (std::size_t j = 0; j <= n; ++j) dp[0][j] = static_cast<int>(j);

    for (std::size_t i = 1; i <= m; ++i) {
        int rowMin = static_cast<int>(i);
        for (std::size_t j = 1; j <= n; ++j) {
            dp[i][j] = std::min({
                dp[i - 1][j] + 1,
                dp[i][j - 1] + 1,
                dp[i - 1][j - 1] + (x[i - 1] == y[j - 1] ? 0 : 1)
            });
            if (i > 1 && j > 1 && x[i - 1] == y[j - 2] && x[i - 2] == y[j - 1]) {
                dp[i][j] = std::min(dp[i][j], dp[i - 2][j - 2] + 1);
            }
            rowMin = std::min(rowMin, dp[i][j]);
        }
        if (rowMin > 3) return 4;
    }
    return dp[m][n];
}

bool MultiDictionary::load(const std::string& path) {
    std::ifstream in(path);
    if (!in) return false;
    words_.clear();
    lookup_.clear();
    std::string line;
    while (std::getline(in, line)) {
        auto word = normalizeWord(line);
        if (word.empty() || word[0] == '#') continue;
        if (lookup_.insert(word).second) words_.push_back(word);
    }
    return !words_.empty();
}

bool MultiDictionary::contains(const std::string& word) const {
    return lookup_.count(normalizeWord(word)) != 0;
}

std::vector<Suggestion> MultiDictionary::suggest(const std::string& q, std::size_t limit, int maxDistance) const {
    std::vector<Suggestion> out;
    const auto query = normalizeWord(q);
    const auto queryCodepoints = decodeUtf8(query);
    for (const auto& word : words_) {
        const auto wordCodepoints = decodeUtf8(word);
        const int distance = editDistanceUnicode(query, word);
        const bool substring = word.find(query) != std::string::npos;
        if (distance <= maxDistance || substring) {
            const int lengthPenalty = std::abs(static_cast<int>(wordCodepoints.size()) -
                                               static_cast<int>(queryCodepoints.size()));
            const int score = distance * 100 + (substring ? -20 : 0) + lengthPenalty;
            out.push_back({word, distance, score});
        }
    }
    std::sort(out.begin(), out.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.score == rhs.score ? lhs.word < rhs.word : lhs.score < rhs.score;
    });
    if (out.size() > limit) out.resize(limit);
    return out;
}
}