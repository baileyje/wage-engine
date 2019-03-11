#ifndef CORE_H
#define CORE_H

#include <string>
#include <vector>
#include <functional>

#include "core/service_map.h"
#include "core/frame.h"
#include "memory/allocator.h"

namespace wage {

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
    void add(T* service) {
      services.add<T>(service);
    }

    template <typename T, typename... Args>
    T* create(Args... args) {
      auto instance = make<T>(args...);
      add<T>(instance);
      return instance;
    }

    template <typename T, typename I, typename... Args>
    I* create(Args... args) {
      auto instance = make<I>(args...);
      add<T>(instance);
      return instance;
    }

    template <typename T>
    inline T* get() {
      return services.get<T>();
    }

    const Frame& frame() const {
      return _frame;
    }

    inline void onInput(UpdateListener listener) {
      inputListeners.push_back(listener);
    }

    inline void onUpdate(UpdateListener listener) {
      updateListeners.push_back(listener);
    }

    void onFixedUpdate(UpdateListener listener) {
      fixedUpdateListeners.push_back(listener);
    }

    inline void onRender(UpdateListener listener) {
      renderListeners.push_back(listener);
    }

  private:
    void processInput();

    void update();

    void fixedUpdate();

    void render();

    void deinit();

    ServiceMap services;

    bool running;

    Frame _frame;

    std::vector<UpdateListener> inputListeners;

    std::vector<UpdateListener> updateListeners;

    std::vector<UpdateListener> fixedUpdateListeners;

    std::vector<UpdateListener> renderListeners;
  };
}
#endif // CORE_H