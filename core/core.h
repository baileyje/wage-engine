#ifndef CORE_H
#define CORE_H

#include <string>
#include <vector>

#include "core/system/map.h"
#include "memory/allocator.h"

namespace wage {

  class System;

  class Core {

  public:

    static Core* Instance;
    
    Core();

    ~Core();
    
    void init();

    void start();
    
    void stop();  

    template <typename T>
    void addSystem(T* system) {  
      systems.add<T>(system);  
      if (running) {
        // system->init(this);      
        start(system);
      }
    }

    template <typename T, typename... Args>
    T* add(Args... args) {  
      auto instance = make<T>(args...);
      addSystem<T>(instance);
      return instance;
    }

    template <typename T, typename I, typename... Args>
    I* add(Args... args) {  
      auto instance = make<I>(args...);
      addSystem<T>(instance);
      return instance;
    }

    template <typename T>
    inline T* get() {
      return systems.get<T>();
    }

    void shouldStop() { 
      // TODO: Make this more clean
      stop();
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

    double timeStep;

    double time;

    double deltaTime;

  };

}
#endif // CORE_H