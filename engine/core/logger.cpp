#include "core/logger.h"

namespace wage {

  LogLevel LogLevel::debug = LogLevel(0, "DEBUG", 32);

  LogLevel LogLevel::info = LogLevel(1, "INFO", 37);

  LogLevel LogLevel::warn = LogLevel(2, "WARN", 33);

  LogLevel LogLevel::error = LogLevel(3, "ERROR", 31);

  LogLevel Logger::CurrentLevel = LogLevel::info;

}