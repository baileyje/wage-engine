#ifndef CORE_H
#define CORE_H

#include <string>
#include <vector>

#include "core/context.h"
#include "fs/local_file_system.h"

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
    // TODO: Make this more clean
    stop();
  }

  inline Scene* getScene() {
    return &scene;
  }

  inline FileSystem* getFileSystem() { 
    return fileSystem;
  }
  
private:

  void update();

  void fixedUpdate();

  void deinit();

  std::vector<System *> systems;
  
  bool running;

  FileSystem* fileSystem;

  Scene scene;

};

#endif // CORE_H