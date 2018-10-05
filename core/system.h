
#ifndef CORE_SYSTEM_H
#define CORE_SYSTEM_H

#include "core/context.h"

class System {

public:

  virtual ~System();

  virtual void init(Context* context);
  
  virtual void start(Context* context);

  virtual void update(Context* context);

  virtual void fixedUpdate(Context* context);
  
  virtual void stop(Context* context);

  virtual void deinit(Context* context);

};

#endif // CORE_SYSTEM_H