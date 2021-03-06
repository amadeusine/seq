#pragma once

#include "util/fmt/format.h"
#include "util/fmt/ostream.h"
#include <iostream>
#include <string>
#include <vector>

#include "lang/seq.h"

#define DBG(c, ...) fmt::print(c "\n", __VA_ARGS__)

namespace seq {
namespace ast {

template <typename T>
std::string combine(const std::vector<T> &items, std::string delim = " ") {
  std::string s = "";
  for (int i = 0; i < items.size(); i++)
    s += (i ? delim : "") + items[i]->to_string();
  return s;
}

std::string escape(std::string s);

std::string executable_path(const char *argv0);

void error(const char *format);
void error(const seq::SrcInfo &p, const char *format);

template <typename... TArgs> void error(const char *format, TArgs &&... args) {
  throw seq::exc::SeqException(fmt::format(format, args...));
}

template <typename... TArgs>
void error(const seq::SrcInfo &p, const char *format, TArgs &&... args) {
  throw seq::exc::SeqException(fmt::format(format, args...), p);
}

std::string getTemporaryVar(const std::string &prefix = "");

template <typename T> T &&fwdSrcInfo(T &&t, const seq::SrcInfo &i) {
  t->setSrcInfo(i);
  return std::forward<T>(t);
}

} // namespace ast
} // namespace seq
