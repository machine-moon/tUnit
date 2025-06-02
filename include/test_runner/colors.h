#pragma once

#include <cstdlib>
#include <string>

namespace tunit {

class Colors {
 public:
  static inline const std::string RESET = "\033[0m";
  static inline const std::string GREEN = "\033[32m";
  static inline const std::string RED = "\033[31m";
  static inline const std::string YELLOW = "\033[33m";
  static inline const std::string BLUE = "\033[34m";
  static inline const std::string MAGENTA = "\033[35m";
  static inline const std::string CYAN = "\033[36m";
  static inline const std::string WHITE = "\033[37m";
  static inline const std::string BOLD = "\033[1m";
  static inline const std::string DIM = "\033[2m";

  static bool colors_enabled();
  static std::string colorize(const std::string& text, const std::string& color);
};

}  // namespace tunit
