#ifndef CORE_H
#define CORE_H

#include <string>
#include <vector>

#include "core/system_map.h"
#include "fs/local_file_system.h"
#include "entity/scene.h" 

class System;

class Core {

public:
  
  Core(std::string rootPath);

  ~Core();
  
  void init();

  void start();
  
  void stop();  

  template <typename T>
  void add(T* system) {  
    systems.add<T>(system);  
    if (running) {
      // system->init(this);      
      start(system);
    }
  }

  template <typename T>
  inline T* get() {
    return systems.get<T>();
  }

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

  inline std::string getRootPath() {
    return rootPath;
  }

  inline double getTimeStep() {
    return timeStep;
  }

  inline double getTime() {
    return time;
  }

  inline double getDeltaTime() {
    return deltaTime;
  }
  
private:

  void update();

  void fixedUpdate();

  void deinit();

  void start(System* system);

  SystemMap systems;
  
  bool running;

  FileSystem* fileSystem;

  Scene scene;

  std::string rootPath;

  double timeStep;

  double time;

  double deltaTime;

};

#endif // CORE_H