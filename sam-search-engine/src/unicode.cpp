#include "unicode.h"
namespace samsearch::unicode {
bool is_unicode_scalar(char32_t c) noexcept { return c <= 0x10FFFF && !(c >= 0xD800 && c <= 0xDFFF); }
std::u32string decode(std::string_view s) {
 std::u32string out; out.reserve(s.size());
 for (size_t i=0;i<s.size();) {
  unsigned char b0=static_cast<unsigned char>(s[i]); char32_t cp=0; size_t n=0;
  if(b0<=0x7F){cp=b0;n=1;} else if(b0>=0xC2&&b0<=0xDF){cp=b0&0x1F;n=2;}
  else if(b0>=0xE0&&b0<=0xEF){cp=b0&0x0F;n=3;} else if(b0>=0xF0&&b0<=0xF4){cp=b0&0x07;n=4;}
  else throw Utf8Error("Invalid UTF-8 leading byte at byte "+std::to_string(i));
  if(i+n>s.size()) throw Utf8Error("Truncated UTF-8 sequence at byte "+std::to_string(i));
  for(size_t j=1;j<n;++j){unsigned char b=static_cast<unsigned char>(s[i+j]);if((b&0xC0)!=0x80)throw Utf8Error("Invalid UTF-8 continuation byte at byte "+std::to_string(i+j));cp=(cp<<6)|(b&0x3F);}
  if((n==2&&cp<0x80)||(n==3&&cp<0x800)||(n==4&&cp<0x10000)||!is_unicode_scalar(cp))throw Utf8Error("Non-canonical or invalid UTF-8 at byte "+std::to_string(i));
  out.push_back(cp);i+=n;
 } return out;
}
std::string encode(std::u32string_view s) {
 std::string out;
 for(char32_t c:s){if(!is_unicode_scalar(c))throw Utf8Error("Invalid Unicode scalar value");
  if(c<=0x7F)out.push_back(static_cast<char>(c));
  else if(c<=0x7FF){out.push_back(static_cast<char>(0xC0|(c>>6)));out.push_back(static_cast<char>(0x80|(c&0x3F)));}
  else if(c<=0xFFFF){out.push_back(static_cast<char>(0xE0|(c>>12)));out.push_back(static_cast<char>(0x80|((c>>6)&0x3F)));out.push_back(static_cast<char>(0x80|(c&0x3F)));}
  else{out.push_back(static_cast<char>(0xF0|(c>>18)));out.push_back(static_cast<char>(0x80|((c>>12)&0x3F)));out.push_back(static_cast<char>(0x80|((c>>6)&0x3F)));out.push_back(static_cast<char>(0x80|(c&0x3F)));}
 } return out;
}
std::u32string normalize_for_search(std::u32string_view s){std::u32string out(s);for(auto&c:out)if(c>=U'A'&&c<=U'Z')c+=U'a'-U'A';return out;}
}