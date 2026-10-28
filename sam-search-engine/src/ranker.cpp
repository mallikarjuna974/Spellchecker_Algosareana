#include "ranker.h"
#include "unicode.h"
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <utility>
namespace samsearch {
void Ranker::load_keywords(const std::string& path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("Cannot open domain keyword file: "+path);std::unordered_set<std::u32string> next;std::string line;while(std::getline(f,line)){if(line.empty()||line[0]=='#')continue;try{auto cps=unicode::normalize_for_search(unicode::decode(line));if(!cps.empty())next.insert(std::move(cps));}catch(const unicode::Utf8Error&e){throw std::runtime_error("Invalid UTF-8 in keyword file: "+std::string(e.what()));}}keywords_=std::move(next);}
void Ranker::set_keywords(std::unordered_set<std::u32string> k){keywords_=std::move(k);}
std::vector<SearchResult> Ranker::rank(std::u32string_view query,const std::vector<Occurrence>& occ,const IndexMap& index,std::size_t radius)const{
 struct Agg{std::size_t count=0;double boost=0;std::string context;};std::unordered_map<std::uint32_t,Agg> agg;const auto q=unicode::normalize_for_search(query);
 for(const auto&o:occ){const Document*d=index.document(o.document_id);if(!d||q.empty()||o.end_position+1<q.size())continue;auto&a=agg[d->id];++a.count;const size_t start=o.end_position+1-q.size();if(a.context.empty()){size_t b=start>radius?start-radius:0;size_t e=std::min(d->codepoints.size(),o.end_position+1+radius);a.context=unicode::encode(std::u32string_view(d->codepoints).substr(b,e-b));}}
 std::vector<SearchResult> out;for(auto&[id,a]:agg){const Document*d=index.document(id);if(!d)continue;auto doc=unicode::normalize_for_search(d->codepoints);for(const auto&kw:keywords_)if(!kw.empty()&&doc.find(kw)!=std::u32string::npos)a.boost+=0.5;double score=1.0+std::min(2.0,static_cast<double>(a.count-1)*0.15)+a.boost;out.push_back({id,score,a.count,d->source,a.context});}
 std::sort(out.begin(),out.end(),[](const auto&a,const auto&b){if(a.score!=b.score)return a.score>b.score;if(a.occurrences!=b.occurrences)return a.occurrences>b.occurrences;return a.source<b.source;});return out;
}
}