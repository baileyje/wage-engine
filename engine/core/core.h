#pragma once

#include <string>
#include <vector>
#include <functional>
#include <thread>

#include "memory/allocator.h"

#include "core/config.h"
#include "core/service_map.h"
#include "core/frame.h"
#include "core/logger.h"

namespace wage {
  namespace core {

    typedef std::function<void(const Frame&)> UpdateListener;

    /**
     * The core of the engine which maintains the continuous update activities. 
     */
    class Core {

    public:
      static Core* Instance;

      /**
       * Initialize the core instance with a specific configuration.
       */
      inline void init(Config config) {
        _frame = {config.fixedTimeStep()};
        Logger::info("Initializing WAGE Engine.");
      }

      /**
       * Start the game engine core service and beging game simulation.
       */
      void start() {
        if (running) return;
        Logger::info("Starting WAGE Engine");
        running = true;
        startServices();
        startUpdateLoop();
        startRenderLoop();
      }

      /**
       * Stop the game engine and shutdown the any services.
       */
      void stop() {
        if (!running) return;
        Logger::info("Stopping WAGE Core.");
        running = false;
        updateThread.join();
        for (auto service : services) {
          Logger::info("Stopping ", service->name().c_str());
          service->stop();
        }
      }

      /**
       * Add a service to the engine with a given type specifier.
       */
      template <typename T>
      void add(T* service) {
        services.add<T>(service);
      }

      /**
       * Create and a service with a specified type arguments.
       */
      template <typename T, typename... Args>
      T* create(Args... args) {
        auto instance = memory::make<T>(args...);
        add<T>(instance);
        return instance;
      }

      /**
       * Create and a service with a specified lookup type as well as a concrete type to construct. This allows for different concrete implementions
       * to be registered with a common generic interface.
       */
      template <typename T, typename I, typename... Args>
      I* create(Args... args) {
        auto instance = memory::make<I>(args...);
        add<T>(instance);
        return instance;
      }

      /**
       * Get the service registered with a specfic type.
       */
      template <typename T>
      inline T* get() {
        return services.get<T>();
      }

      /**
       * Ge the current game time frame.
       */
      inline const Frame& frame() const {
        return _frame;
      }

      /**
       * Add an update listener to be executed at the input phase of the game loop.
       */
      inline void onInput(UpdateListener listener) {
        inputListeners.push_back(listener);
      }

      /**
       * Add an update listener to be executed at the game logic phase of the game loop.
       */
      inline void onUpdate(UpdateListener listener) {
        updateListeners.push_back(listener);
      }

      /**
       * Add an update listener to be executed at the fixed input (physics) phase of the game loop.
       */
      inline void onFixedUpdate(UpdateListener listener) {
        fixedUpdateListeners.push_back(listener);
      }

      /**
       * Add an update listener to be executed at the render phase of the game loop.
       */
      inline void onRender(UpdateListener listener) {
        renderListeners.push_back(listener);
      }

      /**
       * Pause the game execution. 
       */
      inline void pause() {
        _frame.pause();
        paused = true;
      }

      /**
       * Unpause the game execution.
       */
      inline void unpause() {
        _frame.unpause();
        paused = false;
      }

    private:
      void processInput() {
        for (auto listener : inputListeners) {
          listener(frame());
        }
      }

      void update() {
        for (auto listener : updateListeners) {
          listener(frame());
        }
      }

      void fixedUpdate() {
        for (auto listener : fixedUpdateListeners) {
          listener(frame());
        }
      }

      void render() {
        for (auto listener : renderListeners) {
          listener(frame());
        }
      }

      void startUpdateLoop() {
        updateThread = std::thread([&]() {
          double accumulator = 0;
          _frame.reset();
          while (running) {
            if (paused) {
              std::this_thread::sleep_for(std::chrono::seconds(2));
              continue;
            };
            _frame.nextFrame();
            accumulator += _frame.deltaTime();
            while (running && accumulator >= _frame.fixedTimeStep()) {
              fixedUpdate();
              accumulator -= _frame.fixedTimeStep();
            }
            update();
            // Signal to the render that we need the next render queue to fill.
            postUpdate();
          }
        });
      }

      void postUpdate();

      void startRenderLoop() {
        while (running) {
          if (!paused) {
            render();
          }
          processInput();
        }
      }

      inline void startServices() {
        for (auto service : services) {
          Logger::info("Starting ", service->name().c_str());
          service->start();
        }
      }

      ServiceMap services;

      volatile bool running = false;

      volatile bool paused;

      Frame _frame;

      std::vector<UpdateListener> inputListeners;

      std::vector<UpdateListener> updateListeners;

      std::vector<UpdateListener> fixedUpdateListeners;

      std::vector<UpdateListener> renderListeners;

      std::thread updateThread;
    };

  }
}
