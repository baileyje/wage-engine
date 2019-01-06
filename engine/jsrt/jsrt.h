#ifndef JSRT_H
#define JSRT_H

#include "ChakraCore.h"
#include <string>

#include "engine/core/system.h"
#include "engine/core/system/context.h"
#include "engine/jsrt/module_manager.h"
#include "engine/jsrt/js_object_wrapper.h"

namespace wage {

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

    inline SystemContext* getContext() { return context; };

    void attachGlobal(std::string name, JsValueRef valueRef);

  private:

    void attachGlobals();
    
    ModuleManager* moduleManager;

    JsRuntimeHandle jsRuntime;
    
    JsValueRef invoke(std::string source);

    std::queue<Callback*> callbackQueue;

    SystemContext* context;
    
  };

}

#endif // JSRT_H