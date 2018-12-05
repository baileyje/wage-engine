#ifndef JSRT_SYSTEM_H
#define JSRT_SYSTEM_H

#include "core/system.h"

#include "ChakraCore.h"

#include "jsrt/js_object_wrapper.h"

class JsSystem : public System {
  
public:
  
  JsSystem(std::string name, JsValueRef objectRef);

  ~JsSystem();

  LIFECYCLE_FUNC(init)

  LIFECYCLE_FUNC(start)

  LIFECYCLE_FUNC(fixedUpdate)

  LIFECYCLE_FUNC(update)

  LIFECYCLE_FUNC(stop)

  LIFECYCLE_FUNC(deinit)

private:
  
  JsObjectWrapper object;
  
};


#endif //JSRT_SYSTEM_H