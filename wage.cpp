#include "signal.h"
#include <unistd.h>
#include <iostream>
#include <string>

#include "core/core.h"
#include "jsrt/jsrt.h"
#include "physics/physics.h"

Core* core;

void intHandler(int);

// So far so dumb
int main(int argc, char* argv[]) {
  char buffer[255];
  std::string path = std::string(getcwd(buffer, sizeof(buffer)));
  printf("Path: %s\n", path.c_str());
  signal(SIGINT, intHandler);  
  core = new Core(path);  
  core->add(new Jsrt());
  core->add(new Physics());
  core->start();
  delete core;
  return 0;
}

void intHandler(int) { 
  core->stop();
}