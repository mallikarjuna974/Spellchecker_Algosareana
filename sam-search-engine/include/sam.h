#pragma once
#include "index_map.h"
#include <string_view>
#include <unordered_map>
#include <vector>
namespace samsearch {
class SuffixAutomaton {
public:
 struct State { std::size_t max_length{}; int suffix_link{-1}; std::unordered_map<char32_t,StateId> next; bool clone{}; };
 SuffixAutomaton();
 void reset();
 void add_document(std::uint32_t document_id,std::u32string_view text,IndexMap& map);
 int find_state(std::u32string_view query)const;
 const State& state(StateId id)const{return states_.at(id);}
 const std::vector<State>& states()const noexcept{return states_;}
 std::vector<Occurrence> occurrences(StateId query_state,std::size_t limit,const IndexMap& map)const;
private:
 std::vector<State> states_; StateId last_{};
 void extend(char32_t c,bool mark,std::uint32_t doc,std::size_t end_pos,IndexMap& map);
 std::vector<std::vector<StateId>> suffix_link_tree()const;
};
}