#include "core/core.h"

#include <iostream>
#include <chrono>

#include "core/system.h"

typedef std::chrono::high_resolution_clock::time_point TimePoint;

Core::Core(std::string path) : Context(), running(false), fileSystem(new LocalFileSystem(path)) {  
  rootPath = path;
  timeStep = 1.0/60.0;
}

Core::~Core() {
  // TODO: Evaluate whether this makes any sense.
  // for (auto system : systems) {
    // delete system;    
  // }
}

void Core::add(System* system) {  
  systems.push_back(system);  
  if (running) {
    // system->init(this);
    system->start(this);  
  }
}

System* Core::get(std::string name) {
  for (auto system : systems) {
    if (system->getName() == name) {
      return system;
    }
  }
  return NULL;
}

void Core::start() {  
  if (running) {
    return;
  }
  running = true;    
  printf("Starting WAGE Core.\n");
  for (auto system : systems) {
    system->start(this);
  } 
  TimePoint lastTime = std::chrono::high_resolution_clock::now();
  double accumulator = 0;
  while (running) {    
    TimePoint currentTime = std::chrono::high_resolution_clock::now();
    double delta = (std::chrono::duration_cast<std::chrono::duration<double> >(currentTime - lastTime)).count();
    time += delta;
    lastTime = currentTime;
    deltaTime = delta;    
    accumulator += delta;
    if (accumulator >= timeStep) {
      fixedUpdate();
      accumulator -= timeStep;
    }
    // printf("Current: %f - Delta: %f\n", time, delta);
    update();
  }
}

void Core::update() {
  for (auto system : systems) {
    system->update(this);
  }
}

void Core::fixedUpdate() {
  for (auto system : systems) {
    system->fixedUpdate(this);
  }
}

void Core::stop() {
  if(!running) {
    return;
  }
  printf("Stopping WAGE Core.\n");
  running = false;
  for (auto system = systems.rbegin(); system != systems.rend(); ++system) {
    (*system)->stop(this);
  }
  deinit();
}

void Core::init() {
  printf("Initializing WAGE Core.\n");
  for (auto system : systems) {    
    system->init(this);
  }
}

void Core::deinit() {
  printf("Deinitializing WAGE Core.\n");
  for (auto system = systems.rbegin(); system != systems.rend(); ++system) {
    (*system)->deinit(this);
  }
}