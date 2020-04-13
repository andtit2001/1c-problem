#include <array>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>

#include "trie.hpp"
#include "tui_menu/menu.hpp"

using namespace std::string_view_literals;

const std::array<std::basic_string_view<char, tui_menu::ci_char_traits<char>>,
                 4>
    menu_variants = {"Process a text", "Complete a prefix",
                     "Append to the prefix", "Quit"};
const std::regex word_regex("\\w+");

int main() {
  tui_menu::Menu<char> menu(menu_variants.cbegin(), menu_variants.cend());
  smartcompleter::Trie<char, size_t> trie;

  std::string query;
  while (true) {
    auto command = menu.ShowAndAsk(std::cout, std::cin);
    if (command.empty() || command[0] == 'Q') break;
    std::string input;
    if (command[0] == 'P') {
      std::cout << "Enter the text: ";
      std::getline(std::cin, input);
      std::sregex_iterator iter(input.cbegin(), input.cend(), word_regex);
      std::sregex_iterator end;
      for (; iter != end; ++iter) {
        auto word = iter->str();
        auto count_ptr = trie.Get(word.cbegin(), word.cend());
        trie.InsertOrAssign(word.cbegin(), word.cend(),
                            (count_ptr ? *count_ptr : 0) + 1);
      }
    } else {
      if (command[0] == 'C') {
        std::cout << "Enter the prefix: ";
        std::cin >> query;
      } else {
        std::cout << "Enter the 'continuation': ";
        std::cin >> input;
        query += input;
      }
      // TODO: retrieve the most popular word from trie
    }
  }
}
