
#ifndef CORE_SYSTEM_H
#define CORE_SYSTEM_H

#include <string>

#include "core/context.h"

class System {

public:

  System(std::string name) : name(name) {};
  
  virtual ~System() {};

  virtual void init(Context* context) {};
  
  virtual void start(Context* context) {};

  virtual void update(Context* context) {};

  virtual void fixedUpdate(Context* context) {};
  
  virtual void stop(Context* context) {};

  virtual void deinit(Context* context) {};

  virtual std::string getName() { return name; };

private:

  std::string name;

};

#endif // CORE_SYSTEM_H