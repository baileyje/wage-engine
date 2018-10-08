#ifndef CORE_H
#define CORE_H

#include <string>
#include <vector>

#include "core/context.h"


class System;

class Core : public Context {

public:
  
  Core(std::string rootPath);

  ~Core();
  
  void init();

  void start();
  
  void stop();  

  void add(System* system);

  System* get(std::string name);

  void shouldStop() { 
    // TODO: Clean this
    stop();
  }
  
private:

  void update();

  void fixedUpdate();

  void deinit();

  std::vector<System *> systems;
  
  bool running;

};

#endif // CORE_H