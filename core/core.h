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

  virtual EntityStore* getEntities() {
    return &store;
  }

  void add(Entity* entity) {
    store.add(entity);
  }

  inline Entity* getCamera() {
    return &camera;
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

  Entity camera;

  FileSystem* fileSystem;

  EntityStore store;

};

#endif // CORE_H