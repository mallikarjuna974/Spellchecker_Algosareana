#include "Unicode.h"
#include "MultiDictionary.h"
#include <cassert>
#include <iostream>
int main(){
 const std::string te="తెలుగు", ta="தமிழ்", hi="हिन्दी";
 assert(spell::encodeUtf8(spell::decodeUtf8(te))==te);
 assert(spell::encodeUtf8(spell::decodeUtf8(ta))==ta);
 assert(spell::encodeUtf8(spell::decodeUtf8(hi))==hi);
 assert(spell::decodeUtf8("అ").size()==1);
 assert(spell::editDistanceUnicode("అ","ఆ")==1);
 assert(spell::editDistanceUnicode("கல்","கள்")==1);
 assert(spell::editDistanceUnicode("அஆ","ஆஅ")==1); // adjacent transposition
 assert(spell::isSupportedScript(te,"te"));
 assert(spell::isSupportedScript(ta,"ta"));
 assert(spell::isSupportedScript(hi,"hi"));
 assert(!spell::isSupportedScript(te,"ta"));
 std::cout<<"Unicode round-trip and code-point tests passed\n";
}