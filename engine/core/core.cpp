#include "core/core.h"

#include <iostream>
#include <chrono>
#include <thread>

#include "new_ecs/system.h"
#include "core/logger.h"
#include "memory/allocator.h"

namespace wage {

  typedef std::chrono::high_resolution_clock::time_point TimePoint;

  Core* Core::Instance = make<Core>();

  Core::Core() : running(false) {
    _frame._timeStep = 1.0 / 60.0;
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
    TimePoint lastTime = std::chrono::high_resolution_clock::now();
    double accumulator = 0;
    while (running) {
      TimePoint currentTime = std::chrono::high_resolution_clock::now();
      double delta = (std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - lastTime)).count();
      _frame._time += delta;
      lastTime = currentTime;
      _frame._deltaTime = delta;
      accumulator += delta;
      update();
      while (accumulator >= _frame.timeStep()) {
        fixedUpdate();
        accumulator -= _frame.timeStep();
      }
      // Might need to sync/freeze game state here or something..
      render();
      Allocator::Temporary()->clear();
    }
  }

  void Core::processInput() {
    for (auto listener : inputListeners) {
      listener(frame());
    }
  }

  void Core::update() {
    for (auto listener : updateListeners) {
      listener(frame());
    }
  }

  void Core::fixedUpdate() {
    for (auto listener : fixedUpdateListeners) {
      listener(frame());
    }
  }

  void Core::render() {
    for (auto listener : renderListeners) {
      listener(frame());
    }
  }

  void Core::stop() {
    if (!running) {
      return;
    }
    Logger::info("Stopping WAGE Core.");
    running = false;
    for (auto service : services) {
      Logger::info("Stopping ", service->name().c_str());
      service->stop();
    }
  }

  void Core::init() {
    Logger::info("Initializing WAGE Core.");
    // TODO: init Services
  }

  void Core::deinit() {
    Logger::info("Deinitializing WAGE Core.");
    // TODO: deinit Services
  }
}