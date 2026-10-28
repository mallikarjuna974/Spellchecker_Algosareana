#!/usr/bin/env python3
"""Download Indic Project GPLv2 wordlists and select 10,000 unique words per language."""
from pathlib import Path
import sys, urllib.request, unicodedata
ROOT=Path(__file__).resolve().parents[1]; OUT=ROOT/"data"
SOURCES={"te":("te_wordlist.combined",0x0C00,0x0C7F,"Telugu"),"ta":("ta_wordlist.combined",0x0B80,0x0BFF,"Tamil"),"hi":("hi_wordlist.combined",0x0900,0x097F,"Hindi")}
BASE="https://gitlab.com/indicproject/dictionaries/-/raw/master/"
def valid(w,lo,hi):
 letters=[c for c in w if unicodedata.category(c).startswith("L")]
 return bool(letters) and all(lo<=ord(c)<=hi or unicodedata.category(c).startswith("M") for c in letters)
def main():
 OUT.mkdir(exist_ok=True)
 for code,(filename,lo,hi,label) in SOURCES.items():
  url=BASE+filename;print("Downloading",label,url)
  try:
   req=urllib.request.Request(url,headers={"User-Agent":"Spellchecker-Algosareana"})
   with urllib.request.urlopen(req,timeout=45) as r: raw=r.read().decode("utf-8-sig",errors="replace")
  except Exception as e: print("Download failed:",e,file=sys.stderr);return 1
  words=[];seen=set()
  for line in raw.splitlines():
   line=line.strip()
   if not line or line.startswith("#") or "word=" not in line:continue
   w=line.split("word=",1)[1].split(",f=",1)[0].strip()
   if valid(w,lo,hi) and w not in seen:seen.add(w);words.append(w)
   if len(words)==10000:break
  if len(words)!=10000:print(f"Expected 10,000 unique {label} words; found {len(words)}. Refusing to pad with fake words.",file=sys.stderr);return 1
  (OUT/f"{code}.txt").write_text("\n".join(words)+"\n",encoding="utf-8")
  print(f"{label}: {len(words)} words")
 print("Prepared 30,000 entries total.");return 0
if __name__=="__main__":raise SystemExit(main())
