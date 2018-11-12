#include "core/core.h"

#include <iostream>
#include <chrono>

#include "core/context.h"
#include "core/system.h"
#include "core/logger.h"

typedef std::chrono::high_resolution_clock::time_point TimePoint;

Core::Core() : running(false) {  
  timeStep = 1.0/60.0;
}

Core::~Core() {
  // TODO: Evaluate whether this makes any sense.
  // for (auto system : systems) {
    // delete system;    
  // }
}

void Core::start() {  
  if (running) {
    return;
  }
  running = true;    
  Logger::info("Starting WAGE Core.");
  Context context (this);
  for (auto system : systems) {
    Logger::info("Starting ", system->getName().c_str());
    system->start(&context);
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
  Context context(this);
  for (auto system : systems) {
    system->update(&context);
  }
}

void Core::fixedUpdate() {
  Context context(this);
  for (auto system : systems) {
    system->fixedUpdate(&context);
  }
}

void Core::stop() {
  if(!running) {
    return;
  }
  Logger::info("Stopping WAGE Core.");
  running = false;
  Context context(this);
  for (auto system = systems.begin(); system != systems.end(); ++system) {
    (*system)->stop(&context);
  }
  deinit();
}

void Core::init() {
  Logger::info("Initializing WAGE Core.");
  Context context(this);
  for (auto system : systems) {    
    Logger::info("Initializing ", system->getName().c_str());
    system->init(&context);
  }
}

void Core::deinit() {
  Logger::info("Deinitializing WAGE Core.");
  Context context(this);
  for (auto system = systems.begin(); system != systems.end(); ++system) {
    (*system)->deinit(&context);
  }
}

void Core::start(System* system) {
  Context context(this);
  system->start(&context);
}