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

  void add(Entity* entity) {
    entities.push_back(entity);
  }

  std::vector<Entity*>* getEntities() {
    return &entities;
  }

  Camera* getCamera() {
    return &camera;
  }
  
private:

  void update();

  void fixedUpdate();

  void deinit();

  std::vector<System *> systems;
  
  bool running;

  // TODO: Doe these belong here??
  std::vector<Entity*> entities;
  Camera camera;

};

#endif // CORE_H