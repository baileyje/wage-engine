#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include <typeinfo>
#include <typeindex>

#include "core/service.h"

namespace wage { namespace messaging {

  template <typename M>
  class MessageListener {
  public:
    virtual bool on(const M& message) = 0;
  };

  class Messaging : public core::Service {

  public:
    Messaging() : Service("Messaging") {}

    ~Messaging() {}

    template <typename M>
    void send(M& message) {
      auto itr = listeners.find(typeid(M*));
      if (itr != listeners.end()) {
        for (auto listener : itr->second) {
          auto it = static_cast<MessageListener<M>*>(listener);
          if(it->on(message)) {
            return;
          }
        }
      }
    }

    template <typename M>
    void listen(MessageListener<M>* listener) {
      listeners[typeid(M*)].push_back(listener);
    }

  private:
    std::unordered_map<std::type_index, std::vector<void*>> listeners;
  };

} }