#include "engine.h"
#include "unicode.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
namespace samsearch {
void Engine::load_keywords(const std::filesystem::path&p){ranker_.load_keywords(p.string());}
std::uint32_t Engine::add_text(std::string source,std::string text){auto cps=unicode::normalize_for_search(unicode::decode(text));auto id=index_.add_document(std::move(source),std::move(text),std::move(cps));sam_.add_document(id,index_.documents().back().codepoints,index_);return id;}
void Engine::load_text_file(const std::filesystem::path&p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("Cannot open corpus file: "+p.string());std::ostringstream b;b<<f.rdbuf();if(f.bad())throw std::runtime_error("Failed reading corpus file: "+p.string());add_text(p.filename().string(),b.str());}
void Engine::load_corpus_file(const std::filesystem::path&p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("Cannot open corpus file: "+p.string());std::string line;size_t n=0;while(std::getline(f,line)){if(line.empty())continue;add_text(p.filename().string()+":"+std::to_string(++n),line);}if(f.bad())throw std::runtime_error("Failed reading corpus file: "+p.string());}
std::vector<SearchResult> Engine::search(std::string_view q,std::size_t limit)const{auto cps=unicode::normalize_for_search(unicode::decode(q));int s=sam_.find_state(cps);if(s<0)return {};auto occ=sam_.occurrences(static_cast<StateId>(s),limit,index_);return ranker_.rank(cps,occ,index_);}
}