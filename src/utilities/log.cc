#include "utilities/log.h"
#include <iostream>

thread_local size_t Guard::depth_ = 0;

void Guard::Print(const std::string& s) {
  std::cerr << s;
  std::cerr.flush();
}
