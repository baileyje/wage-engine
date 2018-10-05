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

  void init(Context* context);

  void update(Context* context);

  void deinit(Context* context);
  
  void loadModule(std::string name);

  void pushCallback(Callback* callback);

  Core* core;

private:

  void attachGlobals();
  
  ModuleManager* moduleManager;

  JsRuntimeHandle jsRuntime;
  
  JsValueRef invoke(std::string source);

  std::queue<Callback*> callbackQueue;
  
};

#endif // JSRT_H