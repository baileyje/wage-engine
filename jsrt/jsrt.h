#ifndef JSRT_H
#define JSRT_H

#include "ChakraCore.h"
#include <string>

#include "core/system.h"
#include "jsrt/module_manager.h"
#include "jsrt/js_object_wrapper.h"

struct Callback {
	
  JsValueRef function;

};

class Jsrt : public System {

public:
  
  Jsrt();

  ~Jsrt();


  LIFECYCLE_FUNC(init)
  
  LIFECYCLE_FUNC(update)
  
  LIFECYCLE_FUNC(deinit)

  void loadModule(std::string name);

  void pushCallback(Callback* callback);

  Context* getContext() { return context; };

  void attachGlobal(std::string name, JsValueRef valueRef);

private:

  void attachGlobals();
  
  ModuleManager* moduleManager;

  JsRuntimeHandle jsRuntime;
  
  JsValueRef invoke(std::string source);

  std::queue<Callback*> callbackQueue;

  Context* context;
  
};

#endif // JSRT_H