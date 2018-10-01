#include "jsrt.h"
#include <iostream>
#include <string>

#include "jsrt/util.h"
#include "jsrt/module_manager.h"
#include "jsrt/js_object_wrapper.h"

/**
 * Global callback
 **/
static JsValueRef consoleLogCallback(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
static void CHAKRA_CALLBACK promiseContinuationCallback(JsValueRef task, void *callbackState);

Jsrt::Jsrt() {
}

Jsrt::~Jsrt() {
}
  
void Jsrt::init(Context* context) {
  printf("Initializing JSRT.\n");
  
  moduleManager = new ModuleManager(context->rootPath + "/engine/");
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

  loadModule("boot.js");

  JsAddRef(jsRuntime, nullptr);  
}

void Jsrt::start(Context* context) {
  invoke("core.engine.start();");
}

void Jsrt::stop(Context* context) {
  invoke("core.engine.stop();");
}

void Jsrt::deinit(Context* context) {
    printf("Deinitializing JSRT.\n");
    invoke("core.engine.deinit();");
    // FAIL_CHECK(JsSetCurrentContext(JS_INVALID_REFERENCE));
    // FAIL_CHECK(JsDisposeRuntime(jsRuntime));
}

void Jsrt::attachGlobals() {
  JsValueRef globalRef;
  FAIL_CHECK(JsGetGlobalObject(&globalRef));
  JsObjectWrapper global(globalRef);  
  JsObjectWrapper console;
  console.set("log", consoleLogCallback);
  global.set("console", console);  
  global.set("core", JsObjectWrapper());
}

void Jsrt::update(Context* context) {
  // Callback work
  JsValueRef globalRef;
  FAIL_CHECK(JsGetGlobalObject(&globalRef));
  while(!callbackQueue.empty()) {
    printf("Here2!\n");
    Callback* callback = callbackQueue.front();
    callbackQueue.pop();
    JsValueRef result;
    FAIL_CHECK(JsCallFunction(callback->function, &globalRef, 1, &result));
    JsRelease(callback->function, nullptr);
    free(callback);
    
  }
  // Module work
  moduleManager->update();
  // Engine upate
  invoke("core.engine.update();");
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

/**
 * Global callbacks
 * */

JsValueRef consoleLogCallback(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState) {
    printf("-: ");
    for (unsigned int i = 1; i < argumentCount; i++) {
      printf(" ");      
      char* value = stringFromValue(arguments[i]);
      printf("%s", value);
      free(value);
    }
    printf("\n");
    JsValueRef undefinedValue;
    if (JsGetUndefinedValue(&undefinedValue) == JsNoError) {
        return undefinedValue;
    }
    else {
        return nullptr;
    }
}

void promiseContinuationCallback(JsValueRef task, void *callbackState) {
    printf("Prom!\n");
    // Assert(task != JS_INVALID_REFERENCE);
    // Assert(callbackState != JS_INVALID_REFERENCE);
    Jsrt * runtime = (Jsrt *)callbackState;

    Callback* callback = (Callback*)malloc(sizeof(Callback));  
    callback->function = task;

    runtime->pushCallback(callback);

    JsAddRef(task, nullptr);
    // WScriptJsrt::CallbackMessage *msg = new WScriptJsrt::CallbackMessage(0, task);
    // messageQueue->InsertSorted(msg);
}


