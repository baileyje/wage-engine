#ifndef ENTITY_TYPE_INDEX_H
#define ENTITY_TYPE_INDEX_H

#include <atomic>

class TypeIndex {
  
public:
  
  template <typename T>
  static int index() {
    static int id = ++counter;
    return id;
  }

private:

  static std::atomic_int counter;

};

#endif //ENTITY_TYPE_INDEX_H