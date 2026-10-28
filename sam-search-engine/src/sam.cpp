#include "sam.h"
#include <limits>
#include <stdexcept>
#include <utility>
namespace samsearch {
namespace { constexpr char32_t kSeparator=0x110000; }
SuffixAutomaton::SuffixAutomaton(){reset();}
void SuffixAutomaton::reset(){states_.clear();states_.push_back(State{});last_=0;}
void SuffixAutomaton::extend(char32_t c,bool mark,std::uint32_t doc,std::size_t pos,IndexMap& map){
 if(states_.size()>=std::numeric_limits<StateId>::max()-2)throw std::length_error("Suffix automaton state limit exceeded");
 StateId cur=static_cast<StateId>(states_.size());states_.push_back(State{states_[last_].max_length+1,-1,{},false});int p=static_cast<int>(last_);
 while(p!=-1&&!states_[static_cast<size_t>(p)].next.contains(c)){states_[static_cast<size_t>(p)].next[c]=cur;p=states_[static_cast<size_t>(p)].suffix_link;}
 if(p==-1)states_[cur].suffix_link=0;
 else{StateId q=states_[static_cast<size_t>(p)].next[c];if(states_[static_cast<size_t>(p)].max_length+1==states_[q].max_length)states_[cur].suffix_link=static_cast<int>(q);
  else{StateId clone=static_cast<StateId>(states_.size());State copy=states_[q];copy.max_length=states_[static_cast<size_t>(p)].max_length+1;copy.clone=true;states_.push_back(std::move(copy));
   while(p!=-1){auto&edges=states_[static_cast<size_t>(p)].next;auto it=edges.find(c);if(it==edges.end()||it->second!=q)break;it->second=clone;p=states_[static_cast<size_t>(p)].suffix_link;}
   states_[q].suffix_link=static_cast<int>(clone);states_[cur].suffix_link=static_cast<int>(clone);
  }
 }
 last_=cur;if(mark)map.mark_end_state(cur,{doc,pos});
}
void SuffixAutomaton::add_document(std::uint32_t doc,std::u32string_view text,IndexMap& map){for(size_t i=0;i<text.size();++i)extend(text[i],true,doc,i,map);extend(kSeparator,false,doc,0,map);}
int SuffixAutomaton::find_state(std::u32string_view q)const{if(q.empty())return -1;StateId s=0;for(char32_t c:q){auto it=states_[s].next.find(c);if(it==states_[s].next.end())return -1;s=it->second;}return static_cast<int>(s);}
std::vector<std::vector<StateId>> SuffixAutomaton::suffix_link_tree()const{std::vector<std::vector<StateId>> t(states_.size());for(StateId i=1;i<states_.size();++i){int p=states_[i].suffix_link;if(p>=0)t[static_cast<size_t>(p)].push_back(i);}return t;}
std::vector<Occurrence> SuffixAutomaton::occurrences(StateId q,std::size_t limit,const IndexMap& map)const{
 std::vector<Occurrence> out;if(q>=states_.size()||!limit)return out;auto tree=suffix_link_tree();std::vector<StateId> stack{q};
 while(!stack.empty()&&out.size()<limit){StateId s=stack.back();stack.pop_back();for(const auto&o:map.direct_occurrences(s)){const auto*d=map.document(o.document_id);if(!d||o.end_position>=d->codepoints.size())continue;out.push_back(o);if(out.size()>=limit)break;}for(StateId c:tree[s])stack.push_back(c);}
 return out;
}
}