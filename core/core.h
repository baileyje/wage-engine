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
    // TODO: Clean this
    stop();
  }

  void add(Entity* entity) {
    entities.push_back(entity);
  }

  inline std::vector<Entity*>* getEntities() {
    return &entities;
  }

  std::vector<Entity*> getEntitiesWith(std::string componentName) {
    std::vector<Entity*> found;
    for (auto entity : entities) {
      if (entity->getComponent(componentName)) {
        found.push_back(entity);
      }
    }
    return found;
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

  // TODO: Doe these belong here??
  std::vector<Entity*> entities;
  
  Entity camera;

  FileSystem* fileSystem;
};

#endif // CORE_H