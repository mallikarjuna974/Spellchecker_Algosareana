#include "engine.h"
#include "unicode.h"
#include "unicode.h"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
int main(int argc,char**argv){try{
 std::filesystem::path data=argc>1?argv[1]:"data";samsearch::Engine engine;engine.load_keywords(data/"domain_keywords.txt");engine.load_corpus_file(data/"tamil_vocabulary.txt");auto en=data/"english_corpus.txt";if(std::filesystem::exists(en))engine.load_corpus_file(en);
 std::cout<<"SAM multilingual substring search\nDocuments: "<<engine.document_count()<<" | states: "<<engine.state_count()<<" | domain keywords: "<<engine.keyword_count()<<"\n";
 for(const std::string q:{"தமிழ்","கணினி","algorithm","data"}){auto r=engine.search(q,1000);std::cout<<"\nQuery: "<<q<<" ("<<r.size()<<" ranked documents)\n";for(size_t i=0;i<std::min<size_t>(r.size(),5);++i)std::cout<<"  ["<<r[i].score<<"] "<<r[i].source<<" — "<<r[i].context<<"\n";}
 std::cout<<"\nEnter queries (:quit to exit):\n";std::string q;while(std::cout<<"> "&&std::getline(std::cin,q)){if(q==":quit")break;if(q.empty())continue;try{auto r=engine.search(q);if(r.empty()){std::cout<<"No matches.\n";continue;}for(size_t i=0;i<std::min<size_t>(r.size(),10);++i)std::cout<<"["<<r[i].score<<"] "<<r[i].source<<" ("<<r[i].occurrences<<" hits): "<<r[i].context<<"\n";}catch(const samsearch::unicode::Utf8Error&e){std::cerr<<"Query is not valid UTF-8: "<<e.what()<<"\n";}}
 }catch(const std::exception&e){std::cerr<<"Fatal: "<<e.what()<<"\n";return 1;}return 0;}