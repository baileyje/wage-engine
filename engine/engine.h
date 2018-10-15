#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

#include "core/system.h"

#include "entity/entity.h"

class Engine : public System {

public:
  
  Engine() : System("engine") { }

  ~Engine() {}

  void start(Context* context);

  void update(Context* context);

  void fixedUpdate(Context* context);  

};

#endif //ENGINE_H