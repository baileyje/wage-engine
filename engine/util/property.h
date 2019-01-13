#ifndef UTIL_PROPERY_H
#define UTIL_PROPERY_H

#include <memory>

namespace wage {

  template <typename T>
  class Property {
  
  public:

    Property<T>& operator=(T newValue) {
      set(newValue);
      return *this;
    }

    T& operator&() {
      return get();
    }

    T* operator*() {
      return &get();
    }

    T* operator->() {
      return &get();
    }

    T& operator()() {
      return get();
    }

    inline T& get() {
      return value;
    }
    
    inline void set(T newValue) {
      value = newValue;
    }

  private:

    T value;
  };

}

#endif //UTIL_PROPERY_H