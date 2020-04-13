#pragma once
#include <string>
#include <string_view>
#include <variant>

template <class CharT, class Traits = std::char_traits<CharT>>
using StringStorage = std::variant<std::basic_string_view<CharT, Traits>,
                                   std::basic_string<CharT, Traits>>;
