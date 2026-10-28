#include "engine.h"
#include "unicode.h"
#include <cassert>
#include <iostream>
int main(){using namespace samsearch;Engine e;e.add_text("one","prediction algorithm dictionary");e.add_text("two","தமிழ் கணினி அறிவியல்");auto r=e.search("dict");assert(!r.empty());assert(r[0].context.find("dictionary")!=std::string::npos);auto t=e.search("தமிழ்");assert(!t.empty());assert(t[0].context.find("தமிழ்")!=std::string::npos);assert(!e.search("iction").empty());bool threw=false;try{unicode::decode(std::string("\xC0\xAF",2));}catch(const unicode::Utf8Error&){threw=true;}assert(threw);std::cout<<"All SAM search tests passed\n";}