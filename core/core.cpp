#include "core/core.h"

#include <iostream>
#include <chrono>

#include "core/system/context.h"
#include "core/system.h"
#include "core/logger.h"
#include "memory/allocator.h"

namespace wage {

  typedef std::chrono::high_resolution_clock::time_point TimePoint;

  Core* Core::Instance = make<Core>();

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
    Logger::info("Starting WAGE Core");
    SystemContext context (this);
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
      update();
    }
  }

  void Core::update() {
    SystemContext context(this);
    for (auto system : systems) {
      system->update(&context);
    }
    Allocator::Temporary()->clear();
  }

  void Core::fixedUpdate() {
    SystemContext context(this);
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
    SystemContext context(this);
    for (auto system = systems.begin(); system != systems.end(); ++system) {
      Logger::info("Stopping ", (*system)->getName().c_str());
      (*system)->stop(&context);
    }
    deinit();
  }

  void Core::init() {
    Logger::info("Initializing WAGE Core.");
    SystemContext context(this);
    for (auto system : systems) {    
      Logger::info("Initializing ", system->getName().c_str());
      system->init(&context);
    }
  }

  void Core::deinit() {
    Logger::info("Deinitializing WAGE Core.");
    SystemContext context(this);
    for (auto system = systems.begin(); system != systems.end(); ++system) {
      Logger::info("Deinitializing ", (*system)->getName().c_str());
      (*system)->deinit(&context);
    }
  }

  void Core::start(System* system) {
    SystemContext context(this);
    system->start(&context);
  }

}