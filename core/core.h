#ifndef CORE_H
#define CORE_H

#include <string>
#include <vector>



class Context;
class System;

class Core {

public:
  
  Core(std::string rootPath);

  ~Core();
  
  void start();
  
  void stop();  

  void add(System* system);

private:

  void init();

  void update();

  void fixedUpdate();

  void deinit();

  Context* context;

  std::vector<System *> systems;
  
  bool running;

};

#endif // CORE_H