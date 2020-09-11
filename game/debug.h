#pragma once

#include <thread>

#include "engine.h"

using namespace wage;

ecs::Entity logAndReturn(const std::string& filename, const std::uint_fast32_t line, const char* command, ecs::Entity result) {
  // std::cout << filename << ":" << line << " -- " << command << " -- " << result.id().id() << " --" << std::this_thread::get_id() <<  "\n";
  return result;
}

#define IDCHECK(cmd) \
  logAndReturn(__FILE__, __LINE__, #cmd, cmd)
