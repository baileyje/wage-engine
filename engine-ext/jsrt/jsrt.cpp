#include "jsrt.h"
#include <iostream>
#include <string>
#include <sstream>

#include "core/core.h"
#include "core/logger.h"
#include "memory/allocator.h"
#include "jsrt/util.h"
#include "jsrt/module_manager.h"
#include "jsrt/js_object_wrapper.h"
#include "jsrt/js_system.h"

namespace wage {

  /**
   * Global callback
   **/
  static JsValueRef consoleLogCallback(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
  static JsValueRef addSystemCallback(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
  static void CHAKRA_CALLBACK promiseContinuationCallback(JsValueRef task, void *callbackState);

  Jsrt::Jsrt() : System() {
  }

  Jsrt::~Jsrt() {
  }
    
  void Jsrt::init(const SystemContext& context) {
    this->context = context;
    
    moduleManager = make<ModuleManager>(Core::Instance->get<FileSystem>());
    ModuleManager::shared = moduleManager;
    
    JsRuntimeAttributes attrs = static_cast<JsRuntimeAttributes>(JsRuntimeAttributeAllowScriptInterrupt
      | JsRuntimeAttributeDispatchSetExceptionsToDebugger
      | JsRuntimeAttributeEnableExperimentalFeatures
      | JsRuntimeAttributeEnableIdleProcessing);

    JsContextRef jsContext;

    // Create a runtime.
    FAIL_CHECK(JsCreateRuntime(attrs, nullptr, &jsRuntime));
    // Create an execution context.
    FAIL_CHECK(JsCreateContext(jsRuntime, &jsContext));
    // Now set the current execution context.
    FAIL_CHECK(JsSetCurrentContext(jsContext));

    // Setup base module handling
    JsModuleRecord moduleRecord;
    FAIL_CHECK(JsInitializeModuleRecord(NULL, NULL, &moduleRecord));
    FAIL_CHECK(JsSetModuleHostInfo(moduleRecord, JsModuleHostInfo_FetchImportedModuleCallback, (void*)ModuleManager::fetchImportedModule));
    FAIL_CHECK(JsSetModuleHostInfo(moduleRecord, JsModuleHostInfo_FetchImportedModuleFromScriptCallback, (void*)ModuleManager::fetchDynamicImport));
    FAIL_CHECK(JsSetModuleHostInfo(moduleRecord, JsModuleHostInfo_NotifyModuleReadyCallback, (void*)ModuleManager::notifyModuleReady));

    FAIL_CHECK(JsSetPromiseContinuationCallback(promiseContinuationCallback, (void*)this));

    attachGlobals();

    JsAddRef(jsRuntime, nullptr);  
  }

  void Jsrt::deinit(const SystemContext& context) {
      // FAIL_CHECK(JsSetCurrentContext(JS_INVALID_REFERENCE));
      // FAIL_CHECK(JsDisposeRuntime(jsRuntime));
  }

  void attachGlobal(std::string name, JsValueRef valueRef) {
    JsValueRef globalRef;
    FAIL_CHECK(JsGetGlobalObject(&globalRef));
    JsObjectWrapper(globalRef).set(name, valueRef);
  }

  void Jsrt::attachGlobals() {
    JsValueRef globalRef;
    FAIL_CHECK(JsGetGlobalObject(&globalRef));
    JsObjectWrapper global(globalRef);  
    JsObjectWrapper console;
    console.set("log", consoleLogCallback);
    global.set("console", console);  
    JsObjectWrapper core;
    core.set("addSystem", addSystemCallback, this);
    global.set("core", core);  
  }

  void Jsrt::update(const SystemContext& context) {
    // Callback work
    JsValueRef globalRef;
    FAIL_CHECK(JsGetGlobalObject(&globalRef));
    while(!callbackQueue.empty()) {
      Callback* callback = callbackQueue.front();
      callbackQueue.pop();
      JsValueRef result;
      FAIL_CHECK(JsCallFunction(callback->function, &globalRef, 1, &result));
      JsRelease(callback->function, nullptr);
      free(callback);  
    }
    // Module work
    moduleManager->update();
  }

  void Jsrt::loadModule(std::string name) {
    moduleManager->loadModule(NULL, name);
  }

  JsValueRef Jsrt::invoke(std::string source) {
    JsValueRef result;
    FAIL_CHECK(JsRun(valueFromString(source), moduleManager->currentSourceContext++, valueFromString("jsrt"), JsParseScriptAttributeNone, &result));
    return result;
  }

  void Jsrt::pushCallback(Callback* callback) {
    callbackQueue.push(callback);
  }

  void promiseContinuationCallback(JsValueRef task, void *callbackState) {
      // Assert(task != JS_INVALID_REFERENCE);
      // Assert(callbackState != JS_INVALID_REFERENCE);
      Jsrt * runtime = (Jsrt *)callbackState;

      Callback* callback = (Callback*)malloc(sizeof(Callback));  
      callback->function = task;

      runtime->pushCallback(callback);

      JsAddRef(task, nullptr);
  }

  /**
   * Global callbacks
   * */

  JsValueRef consoleLogCallback(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState) {
    std::stringstream message;
    message << "console: ";
    for (unsigned int i = 1; i < argumentCount; i++) {
      char* value = stringFromValue(arguments[i]);
      message << " " << value;
      free(value);
    }
    Logger::debug(message.str());
    JsValueRef undefinedValue;
    if (JsGetUndefinedValue(&undefinedValue) == JsNoError) {
        return undefinedValue;
    }
    else {
        return nullptr;
    }
  }

  JsValueRef addSystemCallback(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState) {
    Logger::debug("Added JS system...");
    Jsrt * runtime = (Jsrt *)callbackState;
    // TODO: Figure out how this should
    // Core::Instance->add<JsSystem>(stringFromValue(arguments[1]),  arguments[2]);
    // TODO: Break this out
    JsValueRef undefinedValue;
    if (JsGetUndefinedValue(&undefinedValue) == JsNoError) {
        return undefinedValue;
    }
    else {
        return nullptr;
    }
  }

}