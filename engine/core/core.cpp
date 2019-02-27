#include "core/core.h"

#include <iostream>
#include <chrono>

#include "core/system/context.h"
#include "ecs/system.h"
#include "core/logger.h"
#include "memory/allocator.h"

namespace wage {

  typedef std::chrono::high_resolution_clock::time_point TimePoint;

  Core* Core::Instance = make<Core>();

  Core::Core() : running(false) {  
    _frame._timeStep = 1.0/60.0;
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
    for (auto service : services) {
      Logger::info("Starting ", service->name().c_str());
      service->start();
    } 
    SystemContext context (this);
    for (auto system : systems) {
      Logger::info("Starting ", system->name().c_str());
      system->start(&context);
    } 
    TimePoint lastTime = std::chrono::high_resolution_clock::now();
    double accumulator = 0;
    while (running) {    
      TimePoint currentTime = std::chrono::high_resolution_clock::now();
      double delta = (std::chrono::duration_cast<std::chrono::duration<double> >(currentTime - lastTime)).count();
      _frame._time += delta;
      lastTime = currentTime;
      _frame._deltaTime = delta;    
      accumulator += delta;
      if (accumulator >= _frame.timeStep()) {
        fixedUpdate();
        accumulator -= _frame.timeStep();
      }
      update();
    }
  }

  void Core::update() {
    SystemContext context(this);
    // for (auto system : systems) {
    //   system->update(&context);
    // }
    for (auto listener : updateListeners) {
      listener(frame());
    }
    Allocator::Temporary()->clear();
  }

  void Core::fixedUpdate() {
    SystemContext context(this);
    for (auto listener : fixedUpdateListeners) {
      listener(frame());
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
      Logger::info("Stopping ", (*system)->name().c_str());
      (*system)->stop(&context);
    }
  }

  void Core::init() {
    Logger::info("Initializing WAGE Core.");
    SystemContext context(this);
    for (auto system : systems) {    
      Logger::info("Initializing ", system->name().c_str());
      system->init(&context);
    }
  }

  void Core::deinit() {
    Logger::info("Deinitializing WAGE Core.");
    SystemContext context(this);
    for (auto system = systems.begin(); system != systems.end(); ++system) {
      Logger::info("Deinitializing ", (*system)->name().c_str());
      (*system)->deinit(&context);
    }
  }

  void Core::start(System* system) {
    SystemContext context(this);
    system->start(&context);
  }

}