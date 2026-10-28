#include "Unicode.h"
#include <cstdint>
namespace spell {
std::vector<char32_t> decodeUtf8(const std::string& s) {
    std::vector<char32_t> out;
    for (std::size_t i=0; i<s.size();) {
        unsigned char c=static_cast<unsigned char>(s[i++]); char32_t cp=0; int n=0;
        if(c<0x80){cp=c;n=0;} else if((c&0xE0)==0xC0){cp=c&0x1F;n=1;}
        else if((c&0xF0)==0xE0){cp=c&0x0F;n=2;} else if((c&0xF8)==0xF0){cp=c&0x07;n=3;}
        else {out.push_back(0xFFFD);continue;}
        bool ok=true;
        for(int j=0;j<n;++j){if(i>=s.size() || (static_cast<unsigned char>(s[i])&0xC0)!=0x80){ok=false;break;}
            cp=(cp<<6)|(static_cast<unsigned char>(s[i++])&0x3F);}
        if(!ok){out.push_back(0xFFFD);continue;}
        if(cp>0x10FFFF || (cp>=0xD800 && cp<=0xDFFF)) cp=0xFFFD;
        out.push_back(cp);
    }
    return out;
}
std::string encodeUtf8(const std::vector<char32_t>& cps) {
    std::string out;
    for(char32_t c:cps) {
        if(c<=0x7F) out.push_back(static_cast<char>(c));
        else if(c<=0x7FF){out.push_back(static_cast<char>(0xC0|(c>>6)));out.push_back(static_cast<char>(0x80|(c&0x3F)));}
        else if(c<=0xFFFF){out.push_back(static_cast<char>(0xE0|(c>>12)));out.push_back(static_cast<char>(0x80|((c>>6)&0x3F)));out.push_back(static_cast<char>(0x80|(c&0x3F)));}
        else {out.push_back(static_cast<char>(0xF0|(c>>18)));out.push_back(static_cast<char>(0x80|((c>>12)&0x3F)));out.push_back(static_cast<char>(0x80|((c>>6)&0x3F)));out.push_back(static_cast<char>(0x80|(c&0x3F)));}
    } return out;
}
std::string normalizeWord(const std::string& s) {
    auto cps=decodeUtf8(s);
    while(!cps.empty() && (cps.front()==' '||cps.front()=='\t'||cps.front()=='\r'||cps.front()=='\n')) cps.erase(cps.begin());
    while(!cps.empty() && (cps.back()==' '||cps.back()=='\t'||cps.back()=='\r'||cps.back()=='\n')) cps.pop_back();
    return encodeUtf8(cps);
}
bool isSupportedScript(const std::string& word,const std::string& lang) {
    auto cps=decodeUtf8(word); bool found=false;
    for(auto c:cps) {
        if(c<128) { if((c>='A'&&c<='Z')||(c>='a'&&c<='z')) return false; continue; }
        bool ok = lang=="te" ? (c>=0x0C00&&c<=0x0C7F) :
                  lang=="ta" ? (c>=0x0B80&&c<=0x0BFF) :
                  lang=="hi" ? (c>=0x0900&&c<=0x097F) : false;
        if(!ok) return false; found=true;
    } return found;
}
}