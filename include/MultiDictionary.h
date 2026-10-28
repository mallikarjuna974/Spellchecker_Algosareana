#pragma once
#include <string>
#include <vector>
#include <unordered_set>
namespace spell {
struct Suggestion { std::string word; int distance; int score; };
class MultiDictionary {
public:
 bool load(const std::string& path);
 bool contains(const std::string& word) const;
 std::vector<Suggestion> suggest(const std::string& query, std::size_t limit=5, int maxDistance=2) const;
 std::size_t size() const { return words_.size(); }
private:
 std::vector<std::string> words_;
 std::unordered_set<std::string> lookup_;
};
int editDistanceUnicode(const std::string& a,const std::string& b);
}