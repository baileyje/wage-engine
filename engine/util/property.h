#ifndef UTIL_PROPERY_H
#define UTIL_PROPERY_H

#include <memory>

namespace wage {

  template <typename T>
  class Property {
  
  public:

    Property<T>& operator=(T newValue) {
      printf("Setting!\n");
      value = newValue;
      return *this;
    }

  private:

    T value;
  };

}

#endif //UTIL_PROPERY_H