#pragma once
#include <istream>
#include <ostream>
#include <vector>

#include "ci_char_traits.hpp"
#include "common.hpp"

namespace tui_menu {

template <class CharT>
class Menu {
 protected:
  std::vector<StringStorage<CharT, ci_char_traits<CharT>>> variant_list;

 public:
  template <class InputIter>
  Menu(InputIter begin, InputIter end) : variant_list(begin, end) {}

  // (This function prints enumerated list, but these indices are useless)
  std::basic_string_view<CharT> ShowAndAsk(
      std::basic_ostream<CharT>& out, std::basic_istream<CharT>& in,
      std::basic_string_view<CharT> preamble = "",
      std::basic_string_view<CharT> prompt = "> ") const {
    out << preamble << '\n';
    // size_t idx = 0;
    for (const auto& var : variant_list) {
      std::visit(
          [/* idx,  */ &out](auto&& value) {
            out << "  * "  //<< "  " << idx << ". "
                << std::basic_string_view<CharT>(value.data(), value.size())
                << '\n';
          },
          var);
      //++idx;
    }
    out << std::endl;

    std::basic_string<CharT> user_input;
    while (true) {
      out << prompt;
      out.flush();

      do {
        std::getline(in, user_input);
        if (in.eof()) return std::basic_string_view<CharT>{};
      } while (user_input.empty());
      std::basic_string_view<CharT, ci_char_traits<CharT>> user_input_view(
          user_input.data(), user_input.size());

      std::basic_string_view<CharT, ci_char_traits<CharT>> selected;
      // Search for exact match
      for (const auto& var : variant_list)
        std::visit(
            [user_input_view, &selected](auto&& value) {
              if (value == user_input_view) selected = value;
            },
            var);
      if (!selected.empty())
        return std::basic_string_view<CharT>(selected.data(), selected.size());

      bool exists = false, ambiguous = false;
      // Search for prefix match
      for (const auto& var : variant_list)
        std::visit(
            [user_input_view, &selected, &exists, &ambiguous](auto&& value) {
              if (value.starts_with(user_input_view))
                if (exists) {
                  ambiguous = true;
                } else {
                  exists = true;
                  selected = value;
                }
            },
            var);

      if (exists && !ambiguous)
        return std::basic_string_view<CharT>(selected.data(), selected.size());

      if (!exists)
        out << "No such prefix. Try again.\n";
      else
        out << "Ambiguous prefix. Try again.\n";
      out.flush();
    }
  }
};

}  // namespace tui_menu
