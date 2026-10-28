#pragma once
#include "index_map.h"
#include "ranker.h"
#include "sam.h"
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>
namespace samsearch {
class Engine {
public:
 void load_keywords(const std::filesystem::path& path);
 std::uint32_t add_text(std::string source,std::string utf8_text);
 void load_text_file(const std::filesystem::path& path);
 void load_corpus_file(const std::filesystem::path& path);
 std::vector<SearchResult> search(std::string_view utf8_query,std::size_t occurrence_limit=10000)const;
 std::size_t document_count()const noexcept{return index_.document_count();}
 std::size_t state_count()const noexcept{return sam_.states().size();}
 std::size_t keyword_count()const noexcept{return ranker_.keyword_count();}
private: SuffixAutomaton sam_;IndexMap index_;Ranker ranker_;
};
}