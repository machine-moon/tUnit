#include "../../include/test_runner/colors.h"

namespace tunit {

bool Colors::colors_enabled() {
  const char* term_env = std::getenv("TERM");
  static bool enabled = (term_env != nullptr && std::string(term_env) != "dumb");
  return enabled;
}

std::string Colors::colorize(const std::string& text, const std::string& color) { 
  return colors_enabled() ? color + text + RESET : text; 
}

}  // namespace tunit
