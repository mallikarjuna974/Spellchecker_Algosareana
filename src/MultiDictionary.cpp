#include "MultiDictionary.h"
#include "Unicode.h"
#include <algorithm>
#include <fstream>
#include <limits>
namespace spell {
int editDistanceUnicode(const std::string& a,const std::string& b) {
 auto x=decodeUtf8(a), y=decodeUtf8(b);
 if(x.size()>y.size()+3 || y.size()>x.size()+3) return 4;
 std::vector<int> prev(y.size()+1),cur(y.size()+1);
 for(std::size_t j=0;j<=y.size();++j) prev[j]=static_cast<int>(j);
 for(std::size_t i=1;i<=x.size();++i) {
   cur[0]=static_cast<int>(i); int rowMin=cur[0];
   for(std::size_t j=1;j<=y.size();++j) {
     cur[j]=std::min({prev[j]+1,cur[j-1]+1,prev[j-1]+(x[i-1]==y[j-1]?0:1)});
     if(i>1&&j>1&&x[i-1]==y[j-2]&&x[i-2]==y[j-1]) cur[j]=std::min(cur[j],(i>2&&j>2?prev[j-2]:prev[j-1])+1);
     rowMin=std::min(rowMin,cur[j]);
   }
   if(rowMin>3) return 4; prev.swap(cur);
 } return prev[y.size()];
}
bool MultiDictionary::load(const std::string& path) {
 std::ifstream in(path); if(!in) return false;
 words_.clear(); lookup_.clear(); std::string line;
 while(std::getline(in,line)) { auto w=normalizeWord(line); if(w.empty()||w[0]=='#') continue;
   if(lookup_.insert(w).second) words_.push_back(w);
 } return !words_.empty();
}
bool MultiDictionary::contains(const std::string& word) const { return lookup_.count(normalizeWord(word))!=0; }
std::vector<Suggestion> MultiDictionary::suggest(const std::string& q,std::size_t limit,int maxD) const {
 std::vector<Suggestion> out; auto query=normalizeWord(q);
 for(const auto& w:words_) {
   auto wc=decodeUtf8(w), qc=decodeUtf8(query);
   int d=editDistanceUnicode(query,w);
   bool substring=(w.find(query)!=std::string::npos);
   if(d<=maxD || substring) {
     int score=d*100 + (substring?-20:0) + std::abs(static_cast<int>(wc.size())-static_cast<int>(qc.size()));
     out.push_back({w,d,score});
   }
 }
 std::sort(out.begin(),out.end(),[](const auto&a,const auto&b){return a.score==b.score?a.word<b.word:a.score<b.score;});
 if(out.size()>limit) out.resize(limit); return out;
}
}