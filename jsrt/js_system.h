#ifndef JSRT_SYSTEM_H
#define JSRT_SYSTEM_H

#include "core/system.h"

#include "core/context.h"

#include "ChakraCore.h"

#include "jsrt/js_object_wrapper.h"

class JsSystem : public System {
  
public:
  
  JsSystem(std::string name, JsValueRef objectRef);

  ~JsSystem();

  void init(Context* context);

  void start(Context* context);

  void fixedUpdate(Context* context);

  void update(Context* context);

  void stop(Context* context);

  void deinit(Context* context);

private:
  
  JsObjectWrapper object;
  
};


#endif //JSRT_SYSTEM_H