#ifndef MESSAGING_H
#define MESSAGING_H

#include <unordered_map>
#include <vector>
#include <functional>
#include <typeinfo>
#include <typeindex>

#include "core/service.h"

namespace wage {
 
  template <typename M>
  class MessageListener {
  public:
    
    virtual void on(M& message) = 0;

  };


  class Messaging : public Service {

  public:

    Messaging() : Service("Messaging") {}

    ~Messaging() {}

    template <typename M>
    void send(M& message) {
      auto itr = listeners.find(typeid(M*));
      if (itr != listeners.end()) {
        for (auto listener : itr->second) {
          auto it = static_cast<MessageListener<M>*>(listener);
          it->on(message);
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

}

#endif //MESSAGING_H