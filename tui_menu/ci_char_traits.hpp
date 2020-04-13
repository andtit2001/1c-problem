#pragma once
#include <cstddef>
#include <locale>
#include <string>

namespace tui_menu {

template <class CharT>
struct ci_char_traits : public std::char_traits<CharT> {
  static CharT to_upper(CharT ch) { return std::toupper(ch, std::locale{}); }
  static bool eq(CharT c1, CharT c2) {
    return std::char_traits<CharT>::eq(to_upper(c1), to_upper(c2));
  }
  static bool lt(CharT c1, CharT c2) {
    return std::char_traits<CharT>::lt(to_upper(c1), to_upper(c2));
  }
  static int compare(const CharT* s1, const CharT* s2, size_t n) {
    for (; n-- != 0; ++s1, ++s2) {
      if (lt(*s1, *s2)) return -1;
      if (lt(*s2, *s1)) return 1;
    }
    return 0;
  }
  static const CharT* find(const CharT* s, int n, CharT a) {
    for (const CharT ua = to_upper(a); n-- != 0; ++s)
      if (eq(to_upper(*s), ua)) return s;
    return nullptr;
  }
};

}  // namespace tui_menu
