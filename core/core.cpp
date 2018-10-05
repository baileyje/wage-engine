#include "core/core.h"

#include <iostream>
#include <chrono>

#include "core/context.h"
#include "core/system.h"

typedef std::chrono::high_resolution_clock::time_point TimePoint;

Core::Core(std::string path) : context(new Context()), running(false) {  
  context->rootPath = path;
  context->core = this;
  context->timeStep = 1.0/60.0;
}

Core::~Core() {
  for (auto system : systems) {
    delete system;
  }
}

void Core::add(System* system) {  
  systems.push_back(system);
  system->init(context);
  if (running) {
    system->start(context);
  }
}

void Core::start() {  
  if (running) {
    return;
  }
  running = true;  
  init();  
  printf("Starting WAGE Core.\n");
  for (auto system : systems) {
    system->start(context);
  } 
  TimePoint lastTime = std::chrono::high_resolution_clock::now();
  double time = 0;
  double accumulator = 0;
  while (running) {    
    TimePoint currentTime = std::chrono::high_resolution_clock::now();
    double delta = (std::chrono::duration_cast<std::chrono::duration<double> >(currentTime - lastTime)).count();
    time += delta;
    lastTime = currentTime;
    context->time = time;
    context->deltaTime = delta;    
    accumulator += delta;
    if (accumulator >= context->timeStep) {
      fixedUpdate();
      accumulator -= context->timeStep;
    }
    // printf("Current: %f - Delta: %f\n", time, delta);
    update();
  }  
}

void Core::update() {
  for (auto system : systems) {
    system->update(context);
  }
}

void Core::fixedUpdate() {
  for (auto system : systems) {
    system->fixedUpdate(context);
  }
}

void Core::stop() {
  if(!running) {
    return;
  }  
  printf("Stopping WAGE Core.\n");
  running = false;  
  for (auto system : systems) {
    system->stop(context);
  }
  deinit();
}

void Core::init() {
  printf("Initializing WAGE Core.\n");
  for (auto system : systems) {    
    system->init(context);
  }
}

void Core::deinit() {
  printf("Deinitializing WAGE Core.\n");
  for (auto system : systems) {
    system->deinit(context);
  }
}