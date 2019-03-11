#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <cstdio>
#include <iostream>

#define LeveledLog(Level)     \
  template <typename... Args> \
  static void Level(std::string message, Args... args) { log(LogLevel::Level, message, args...); }

namespace wage {

  class LogLevel {

  public:
    static LogLevel debug;

    static LogLevel info;

    static LogLevel warn;

    static LogLevel error;

    LogLevel(int severity, std::string name, int colorCode) : _severity(severity), _name(name), _colorCode(colorCode) {
    }

    inline int severity() { return _severity; }

    inline std::string name() { return _name; }

    inline int colorCode() { return _colorCode; }

  private:
    int _severity;

    std::string _name;

    int _colorCode;
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
      return level.name().c_str();
    }

    static int colorForLevel(LogLevel level) {
      return level.colorCode();
    }

    template <typename... Args>
    static void log(LogLevel level, std::string& message, Args... args) {
      if (level.severity() >= CurrentLevel.severity()) {
        std::cout << "\033[" << colorForLevel(level) << ";1m" << nameForLevel(level) << "\033[0m ";
        write(message, args...);
        std::cout << std::endl;
      }
    }
  };
}

#endif // LOGGER_H