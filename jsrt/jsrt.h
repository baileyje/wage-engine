#ifndef JSRT_H
#define JSRT_H

#include "ChakraCore.h"
#include <string>

#include "core/system.h"
#include "jsrt/module_manager.h"

class Jsrt : public System {

public:
  
  Jsrt();

  ~Jsrt();

  void init(Context* context);
  
  void start(Context* context);
  
  void update(Context* context);

  void stop(Context* context);

  void deinit(Context* context);
  
  void loadModule(std::string name);

private:

  void attachGlobals();
  
  ModuleManager* moduleManager;

  JsRuntimeHandle jsRuntime;
  
  JsValueRef invoke(std::string source);

  JsValueRef undefinedValue;
};

#endif // JSRT_H