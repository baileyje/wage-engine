#ifndef CORE_H
#define CORE_H

#include <string>
#include <vector>
#include <functional>

#include "core/system/map.h"
#include "core/service_map.h"
#include "core/frame.h"
#include "memory/allocator.h"

namespace wage {

  class System;

  typedef std::function<void(const Frame&)> UpdateListener;

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
    void addService(T* system) {  
      services.add<T>(system);  
    }

    template <typename T, typename... Args>
    T* create(Args... args) {  
      auto instance = make<T>(args...);
      addService<T>(instance);
      return instance;
    }

    template <typename T, typename I, typename... Args>
    I* create(Args... args) {  
      auto instance = make<I>(args...);
      addService<T>(instance);
      return instance;
    }

    template <typename T>
    inline T* get() {
      auto result = services.get<T>();
      if (result) {
        return result;
      }
      return systems.get<T>();
    }

    void shouldStop() { 
      // TODO: Make this more clean
      stop();
    }

    const Frame& frame() const {
      return _frame;
    }    

    void onUpdate(UpdateListener listener) {
      updateListeners.push_back(listener);
    }

    void onFixedUpdate(UpdateListener listener) {
      fixedUpdateListeners.push_back(listener);
    }
    
  private:

    void update();

    void fixedUpdate();

    void deinit();

    void start(System* system);

    SystemMap systems;

    ServiceMap services;
    
    bool running;

    Frame _frame;

    std::vector<UpdateListener> updateListeners;

    std::vector<UpdateListener> fixedUpdateListeners;

  };

}
#endif // CORE_H