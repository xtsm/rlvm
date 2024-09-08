#pragma once
#include <sstream>
#include <string>

class Guard {
 public:
  template <class... Args>
  Guard(const char* file, size_t line, const char* func) {
    std::ostringstream ss;
    for (size_t i = 0; i < depth_; ++i) {
      ss << "  ";
    }
    ss << "ENTER " << func << ' ' << file << ':' << line << '\n';
    ++depth_;
    Print(ss.str());
  }
  template <class... Args>
  static void Log(const char* file, size_t line, Args... args) {
    std::ostringstream ss;
    for (size_t i = 0; i < depth_; ++i) {
      ss << "  ";
    }
    ss << file << ':' << line;
    ((ss << ' ' << std::forward<Args>(args)), ...);
    ss << '\n';
    Print(ss.str());
  }
  ~Guard() {
    --depth_;
  }

  static void Print(const std::string&);

 private:
  static thread_local size_t depth_;
};

#define GUARD(name) Guard __guard(__FILE__, __LINE__, name)
#define GUARDFUNC GUARD(__func__)
#define LOG(...) Guard::Log(__FILE__, __LINE__, __VA_ARGS__)
