#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <cstdio>
#include <iostream>

#define LeveledLog(Level) \
  template<typename... Args> \
  static void Level(std::string message, Args... args) { log(LogLevel::Level, message, args...); }

namespace wage {

  class LogLevel {

  public:

    static LogLevel debug;

    static LogLevel info;

    static LogLevel warn;

    static LogLevel error;

    LogLevel(int severity, std::string name, int colorCode) : severity(severity), name(name), colorCode(colorCode) {
    }

    inline int getSeverity() { return severity; }

    inline std::string getName() { return name; }

    inline int getColorCode() { return colorCode; }

  private:

    int severity;

    std::string name;

    int colorCode;

  };

  class Logger {

  public:

    static LogLevel CurrentLevel;

    LeveledLog(debug)
    
    LeveledLog(info)

    LeveledLog(warn)

    LeveledLog(error)
  
    template <typename Arg>
    static void write(Arg arg) {
      std::cout << arg;
    }

    template <typename Arg, typename... Args>
    static void write(Arg arg, Args... args) {
      std::cout << arg;
      write(args...);
    }

    static const char* nameForLevel(LogLevel level) {
      // static const char* names[] = {
      //   "DEBUG",
      //   "INFO",
      //   "WARN",
      //   "ERROR"
      // };
      // int levelIdx = static_cast<std::underlying_type<LogLevel>::type>(level);
      return level.getName().c_str();
    }

    static int colorForLevel(LogLevel level) {
      // switch(level) {
      //   case LogLevel::error: return 31;
      //   case LogLevel::warn: return 33;
      //   default: return 32;
      // }
      return level.getColorCode();
    }

    template <typename... Args>
    static void log(LogLevel level, std::string& message, Args... args) {
      if (level.getSeverity() >= CurrentLevel.getSeverity()) {      
        std::cout << "\033[" << colorForLevel(level) << ";1m" << nameForLevel(level) << "\033[0m ";
        write(message, args...);
        std::cout << std::endl;
      }
    }  
  };

}

#endif // LOGGER_H