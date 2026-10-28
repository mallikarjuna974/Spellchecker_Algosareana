#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
namespace samsearch {
using StateId=std::uint32_t;
struct Document { std::uint32_t id{}; std::string source; std::string text; std::u32string codepoints; };
struct Occurrence { std::uint32_t document_id{}; std::size_t end_position{}; };
class IndexMap {
public:
 std::uint32_t add_document(std::string source,std::string text,std::u32string codepoints);
 void mark_end_state(StateId state,Occurrence occurrence);
 const std::vector<Occurrence>& direct_occurrences(StateId state) const;
 const Document* document(std::uint32_t id) const;
 const std::vector<Document>& documents()const noexcept{return documents_;}
 std::size_t document_count()const noexcept{return documents_.size();}
private:
 std::vector<Document> documents_;
 std::unordered_map<StateId,std::vector<Occurrence>> terminals_;
};
}