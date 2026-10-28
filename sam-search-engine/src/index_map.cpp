#include "index_map.h"
#include <stdexcept>
#include <utility>
namespace samsearch {
std::uint32_t IndexMap::add_document(std::string source,std::string text,std::u32string cps){
 if(documents_.size()>=UINT32_MAX)throw std::overflow_error("Too many documents");
 auto id=static_cast<std::uint32_t>(documents_.size());documents_.push_back({id,std::move(source),std::move(text),std::move(cps)});return id;
}
void IndexMap::mark_end_state(StateId s,Occurrence o){terminals_[s].push_back(o);}
const std::vector<Occurrence>& IndexMap::direct_occurrences(StateId s)const{static const std::vector<Occurrence> empty;auto i=terminals_.find(s);return i==terminals_.end()?empty:i->second;}
const Document* IndexMap::document(std::uint32_t id)const{return id<documents_.size()?&documents_[id]:nullptr;}
}