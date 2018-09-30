#include "core/core.h"

#include <iostream>

Core::Core(std::string path) : running(false) {  
  context.rootPath = path;
}

Core::~Core() {
  for (auto system : systems) {
    delete system;
  }
}

void Core::add(System* system) {  
  systems.push_back(system);
}

void Core::start() {  
  if (running) {
    return;
  }
  running = true;  
  init();  
  printf("Starting WAGE Core.\n");
  for (auto system : systems) {
    system->start(&context);
  }  
  while (running) {
    update();
  }  
}

void Core::update() {
  for (auto system : systems) {
    system->update(&context);
  }
}

void Core::stop() {
  if(!running) {
    return;
  }  
  printf("Stopping WAGE Core.\n");
  running = false;  
  for (auto system : systems) {
    system->stop(&context);
  }
  deinit();
}

void Core::init() {
  printf("Initializing WAGE Core.\n");
  for (auto system : systems) {    
    system->init(&context);
  }
}

void Core::deinit() {
  printf("Deinitializing WAGE Core.\n");
  for (auto system : systems) {
    system->deinit(&context);
  }
}