#pragma once
#include <string>
#include <vector>
namespace spell {
std::vector<char32_t> decodeUtf8(const std::string& text);
std::string encodeUtf8(const std::vector<char32_t>& codepoints);
std::string normalizeWord(const std::string& text);
bool isSupportedScript(const std::string& word, const std::string& language);
}