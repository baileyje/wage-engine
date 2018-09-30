#include "jsrt.h"
#include <iostream>
#include <string>

#include "jsrt/util.h"
#include "jsrt/module_manager.h"

/**
 * Global callback
 **/
static JsValueRef consoleLogCallback(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);

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

  attachGlobals();

  loadModule("boot.js");

  JsAddRef(jsRuntime, nullptr);  
}


void Jsrt::start(Context* context) {
  invoke("core.engine.start();");
}

void Jsrt::stop(Context* context) {
  printf("WTF!!\n");
  invoke("core.engine.stop();");
}

void Jsrt::deinit(Context* context) {
    printf("Deinitializing JSRT.\n");
    invoke("core.engine.deinit();");
    // FAIL_CHECK(JsSetCurrentContext(JS_INVALID_REFERENCE));
    // FAIL_CHECK(JsDisposeRuntime(jsRuntime));
}

void Jsrt::attachGlobals() {
  JsValueRef global;
  FAIL_CHECK(JsGetGlobalObject(&global));
  
  JsValueRef console;
  FAIL_CHECK(JsCreateObject(&console));
  installObjectsOnObject(console, "log", consoleLogCallback);

  JsPropertyIdRef consoleName;
  FAIL_CHECK(JsCreatePropertyId("console", strlen("console"), &consoleName));
  FAIL_CHECK(JsSetProperty(global, consoleName, console, true));

  JsValueRef core;
  FAIL_CHECK(JsCreateObject(&core));
  JsPropertyIdRef coreName;
  FAIL_CHECK(JsCreatePropertyId("core", strlen("core"), &coreName));
  FAIL_CHECK(JsSetProperty(global, coreName, core, true));
}

void Jsrt::update(Context* context) {
  // Module work first
  moduleManager->update();
  // Engine next
  invoke("core.engine.update();");
}

void Jsrt::loadModule(std::string name) {
  moduleManager->loadModule(NULL, name.c_str());
}

JsValueRef Jsrt::invoke(std::string source) {
  JsValueRef result;
  FAIL_CHECK(JsRun(valueFromString(source.c_str()), moduleManager->currentSourceContext++, valueFromString("jsrt"), JsParseScriptAttributeNone, &result));
  return result;
}

/**
 * Global callbacks
 * */

static JsValueRef consoleLogCallback(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState) {
    printf("-: ");
    for (unsigned int i = 1; i < argumentCount; i++) {
      printf(" ");
      JSString value(arguments[i]);
      printf("%s", value.getCString());
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


