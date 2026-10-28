#pragma once
#include "index_map.h"
#include <string>
#include <unordered_set>
#include <vector>
namespace samsearch {
struct SearchResult { std::uint32_t document_id{}; double score{}; std::size_t occurrences{}; std::string source; std::string context; };
class Ranker {
public:
 void load_keywords(const std::string& path);
 void set_keywords(std::unordered_set<std::u32string> keywords);
 std::vector<SearchResult> rank(std::u32string_view query,const std::vector<Occurrence>& occurrences,const IndexMap& index,std::size_t context_radius=40)const;
 std::size_t keyword_count()const noexcept{return keywords_.size();}
private: std::unordered_set<std::u32string> keywords_;
};
}