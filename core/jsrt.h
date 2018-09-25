
#ifndef JSRT_H
#define JSRT_H

#include "include/ChakraCore.h"

class ModuleManager;

class Jsrt {

public:
  
  Jsrt();

  ~Jsrt();
  
  void init();

  void deinit();

  void update();

private:

  void attachGlobals();
  
  ModuleManager* moduleManager;

  JsRuntimeHandle runtime;
};

#endif // JSRT_H