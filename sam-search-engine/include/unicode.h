#pragma once
#include <string>
#include <string_view>
#include <stdexcept>
namespace samsearch::unicode {
class Utf8Error : public std::runtime_error { public: using std::runtime_error::runtime_error; };
// Strict UTF-8 validation; rejects overlong sequences, surrogates, truncation and invalid scalars.
std::u32string decode(std::string_view input);
std::string encode(std::u32string_view input);
std::u32string normalize_for_search(std::u32string_view input); // ASCII case-fold only; Tamil preserved.
bool is_unicode_scalar(char32_t cp) noexcept;
}