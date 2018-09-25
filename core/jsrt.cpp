#include "jsrt.h"
#include <iostream>
#include <string>
#include "include/ChakraCore.h"

using namespace std;

#define FAIL_CHECK(cmd)                     \
    do                                      \
    {                                       \
        JsErrorCode errCode = cmd;          \
        if (errCode != JsNoError)           \
        {                                   \
            printf("Error %s at '%s'\n",    \
              jsErrorCodeToString(errCode).c_str(), #cmd);             \
        }                                   \
    } while(0)

/**
 * Register utils
 **/
static string jsErrorCodeToString(JsErrorCode jsErrorCode);
static bool  installObjectsOnObject(JsValueRef object, const char* name, JsNativeFunction nativeFunction);
static bool createNamedFunction(const char* nameString, JsNativeFunction callback, JsValueRef* functionVar);
static char* stringFromValue(JsValueRef value);

/**
 * Global callback
 **/
static JsValueRef consoleLogCallback(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);


struct LoadTask {
	JsModuleRecord  module;
	char*           source;
  size_t          sourceLength;
	JsSourceContext sourceContext;	
};

struct LoadedModule {
	JsModuleRecord  module;
	const char*           path;
};

class ModuleManager {

public:
  ModuleManager();

  ~ModuleManager();

  void update();

  static JsModuleRecord loadModule(JsModuleRecord importer, const char* path);
  
  static JsModuleRecord createModule(const char* specifier, JsModuleRecord parentRecord, const char* url, bool *out_is_new);

  void parseModule(LoadTask* task);

  void evaluateModule(LoadTask* task);

  static JsErrorCode CHAKRA_CALLBACK fetchImportedModule(JsModuleRecord importer, JsValueRef specifier, JsModuleRecord *outModule);

  static JsErrorCode CHAKRA_CALLBACK fetchDynamicImport(JsSourceContext importer, JsValueRef specifier, JsModuleRecord *outModule);
  
  static JsErrorCode CHAKRA_CALLBACK notifyModuleReady(JsModuleRecord module, JsValueRef exception);
  
  // TODO: JUNK HACK FEST 99
  static char* readFile(const char* path);

private: 
  static unsigned currentSourceContext;
  
  static LoadTask* moduleLoadQueue[128];
  
  static int moduleLoadQueueIdx;

  static void pushLoadTask(LoadTask* task);

  static LoadTask* popLoadTask();

  static LoadedModule* loadedModules[128];

  static int loadedModulesLength;
};


Jsrt::Jsrt() : moduleManager(new ModuleManager()) {
}

Jsrt::~Jsrt() {
  delete moduleManager;
}
  
void Jsrt::init() {
  printf("Initializing JSRT.\n");
  JsRuntimeHandle jsRuntime;
  JsContextRef context;    

  JsRuntimeAttributes attrs = static_cast<JsRuntimeAttributes>(JsRuntimeAttributeAllowScriptInterrupt
    | JsRuntimeAttributeDispatchSetExceptionsToDebugger
    | JsRuntimeAttributeEnableExperimentalFeatures
    | JsRuntimeAttributeEnableIdleProcessing);

  // Create a runtime.
  FAIL_CHECK(JsCreateRuntime(attrs, nullptr, &jsRuntime));
  // Create an execution context.
  FAIL_CHECK(JsCreateContext(jsRuntime, &context));
  // Now set the current execution context.
  FAIL_CHECK(JsSetCurrentContext(context));

  // Setup base module
  JsModuleRecord moduleRecord;
  FAIL_CHECK(JsInitializeModuleRecord(NULL, NULL, &moduleRecord));
  FAIL_CHECK(JsSetModuleHostInfo(moduleRecord, JsModuleHostInfo_FetchImportedModuleCallback, (void*)ModuleManager::fetchImportedModule));
  FAIL_CHECK(JsSetModuleHostInfo(moduleRecord, JsModuleHostInfo_FetchImportedModuleFromScriptCallback, (void*)ModuleManager::fetchDynamicImport));
  FAIL_CHECK(JsSetModuleHostInfo(moduleRecord, JsModuleHostInfo_NotifyModuleReadyCallback, (void*)ModuleManager::notifyModuleReady));
  runtime = jsRuntime;
  attachGlobals();
  ModuleManager::loadModule(NULL, "core.js");    
}

void Jsrt::deinit() {
    FAIL_CHECK(JsSetCurrentContext(JS_INVALID_REFERENCE));
    FAIL_CHECK(JsDisposeRuntime(runtime));
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
}

void Jsrt::update() {
  moduleManager->update();
}


/**
 * Global callbacks
 * */

static JsValueRef consoleLogCallback(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState) {
    printf("Console:");
    for (unsigned int i = 1; i < argumentCount; i++) {
      printf(" ");
      char* valueStr = stringFromValue(arguments[i]); 
      printf("%s", valueStr);
      free(valueStr);
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

/**
 * Module Manager Impl
 **/

unsigned ModuleManager::currentSourceContext;

LoadTask* ModuleManager::moduleLoadQueue[128];

int ModuleManager::moduleLoadQueueIdx;

LoadedModule* ModuleManager::loadedModules[128];

int ModuleManager::loadedModulesLength;

ModuleManager::ModuleManager() {
  moduleLoadQueueIdx = -1;
  currentSourceContext = 0;
  loadedModulesLength = 0;
}

ModuleManager::~ModuleManager() {
  for (int i = 0; i < moduleLoadQueueIdx; i++) {
    LoadTask* task = moduleLoadQueue[i];
    if (task->source != NULL) {
      free(task->source);
    }
    free(task);
  }
  for (int i = 0; i < loadedModulesLength; i++) {
    free(loadedModules[i]);
  }
}

void ModuleManager::update() {
  while(moduleLoadQueueIdx > -1) {
    LoadTask* task = popLoadTask();
    if (task->source != NULL) {
      parseModule(task);
      free(task->source);          
    } else {
      evaluateModule(task);
    }
    free(task);
  }
}

JsModuleRecord ModuleManager::loadModule(JsModuleRecord importer, const char* path) {
  JsValueRef sepcifier_ref;
  JsCreateString(path, strlen(path), &sepcifier_ref);
  bool is_new_module;

  JsModuleRecord module_record = createModule(path, importer, path, &is_new_module);
  
  LoadedModule* loaded = (LoadedModule*)malloc(sizeof(LoadedModule));
  loaded->module = module_record;
  loaded->path = path;  
  loadedModules[loadedModulesLength++] = loaded;
  
  char* source = readFile(path);
  LoadTask* task = (LoadTask*)malloc(sizeof(LoadTask));  
  task->sourceContext = currentSourceContext++;
  task->module = module_record;
  task->source = source;
  task->sourceLength = strlen(source);
  ModuleManager::pushLoadTask(task);  
  return module_record; 
}

JsModuleRecord ModuleManager::createModule(const char* specifier, JsModuleRecord parent_record, const char* url, bool *out_is_new) {
  // printf("Module: %s\n", specifier);
  JsModuleRecord module_record;
  JsValueRef     specifier_ref;
	JsValueRef     url_ref;
  *out_is_new = true;
	JsCreateString(specifier, strlen(specifier), &specifier_ref);
	JsCreateString(url, strlen(url), &url_ref);
  JsInitializeModuleRecord(parent_record, specifier_ref, &module_record);
  JsSetModuleHostInfo(module_record, JsModuleHostInfo_HostDefined, specifier_ref);
  // Setup callbacks
  JsSetModuleHostInfo(module_record, JsModuleHostInfo_FetchImportedModuleCallback, (void*)ModuleManager::fetchImportedModule);
  JsSetModuleHostInfo(module_record, JsModuleHostInfo_FetchImportedModuleFromScriptCallback, (void*)ModuleManager::fetchDynamicImport);
  JsSetModuleHostInfo(module_record, JsModuleHostInfo_NotifyModuleReadyCallback, (void*)ModuleManager::notifyModuleReady);
  JsAddRef(module_record, NULL);
  return module_record;
}

void ModuleManager::parseModule(LoadTask* task) {
  JsValueRef exception;
  // printf("SRC %s\n", task->source);
  JsErrorCode  error_code = JsParseModuleSource(task->module, task->sourceContext, (BYTE*)task->source, (unsigned int)task->sourceLength, JsParseModuleSourceFlags_DataIsUTF8, &exception);
  if (error_code == JsErrorScriptCompile) {
    JsPropertyIdRef linePropertyId = JS_INVALID_REFERENCE;
    JsValueRef lineProperty = JS_INVALID_REFERENCE;

    JsPropertyIdRef columnPropertyId = JS_INVALID_REFERENCE;
    JsValueRef columnProperty = JS_INVALID_REFERENCE;

    int line;
    int column;
    JsCreatePropertyId("line", strlen("line"),  &linePropertyId);
    JsGetProperty(exception, linePropertyId, &lineProperty);
    JsNumberToInt(lineProperty, &line);

    JsCreatePropertyId("column", strlen("column"),  &columnPropertyId);
    JsGetProperty(exception, columnPropertyId, &columnProperty);
    JsNumberToInt(columnProperty, &column);

    char *exceptionSTR = stringFromValue(exception);
    printf("Exception -> %s \n", exceptionSTR);
    printf("At: %d:%d\n", line, column);
    free(exceptionSTR);
  }    
  // FAIL_CHECK(error_code);
}

void ModuleManager::evaluateModule(LoadTask* task) {
  JsValueRef result;
  FAIL_CHECK(JsModuleEvaluation(task->module, &result));  
}

JsErrorCode CHAKRA_CALLBACK ModuleManager::fetchImportedModule(JsModuleRecord importer, JsValueRef module_name, JsModuleRecord *outModule) {
  char* moduleName = stringFromValue(module_name); 
  // printf("Importing - %s\n", moduleName);
  JsModuleRecord module_record = loadModule(importer, moduleName);  
  *outModule = module_record;
  free(moduleName);
  return JsNoError;
}

JsErrorCode CHAKRA_CALLBACK ModuleManager::fetchDynamicImport(JsSourceContext importer, JsValueRef specifier, JsModuleRecord *outModule) {
  // printf("Dynamic Import - %s\n", specifier);
	return fetchImportedModule(NULL, specifier, outModule);
}
  
JsErrorCode CHAKRA_CALLBACK ModuleManager::notifyModuleReady(JsModuleRecord module, JsValueRef exception) {
  // printf("Module Ready\n");
  LoadTask* task = (LoadTask*)malloc(sizeof(LoadTask));  
  task->module = module;
  task->source = NULL;
  pushLoadTask(task);    
	return JsNoError;
}

// TODO: JUNK HACK FEST 99
char* ModuleManager::readFile(const char* path) {
  FILE* f = fopen(path, "r");
  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);
  char* data = new char[size];
  rewind(f);
  fread(data, sizeof(char), size, f);
  fclose(f);
  return data; 
}

void ModuleManager::pushLoadTask(LoadTask* task) {
  // printf("-> %zu\n", dup->sourceLength);
  moduleLoadQueue[++moduleLoadQueueIdx] = task;
}

LoadTask* ModuleManager::popLoadTask() {
  return moduleLoadQueue[moduleLoadQueueIdx--];
}

/**
 * CC Utils
 * */

string jsErrorCodeToString(JsErrorCode jsErrorCode) {
    bool hasException = false;
    JsHasException(&hasException);
    printf("Exc: %d\n", hasException);
    if (hasException) {
      JsValueRef exception;
      JsGetAndClearException(&exception);
      JsValueRef exceptionJSString;
      JsConvertValueToString(exception, &exceptionJSString);

      JsPropertyIdRef linePropertyId = JS_INVALID_REFERENCE;
      JsValueRef lineProperty = JS_INVALID_REFERENCE;

      JsPropertyIdRef columnPropertyId = JS_INVALID_REFERENCE;
      JsValueRef columnProperty = JS_INVALID_REFERENCE;

      int line;
      int column;
      JsCreatePropertyId("line", strlen("line"),  &linePropertyId);
      JsGetProperty(exception, linePropertyId, &lineProperty);
      JsNumberToInt(lineProperty, &line);

      JsCreatePropertyId("column", strlen("column"),  &columnPropertyId);
      JsGetProperty(exception, columnPropertyId, &columnProperty);
      JsNumberToInt(columnProperty, &column);

      char *exceptionSTR = nullptr;
      size_t stringLength;
      JsCopyString(exceptionJSString, nullptr, 0, &stringLength);
      exceptionSTR = (char*) malloc(stringLength + 1);
      JsCopyString(exceptionJSString, exceptionSTR, stringLength + 1, nullptr);
      exceptionSTR[stringLength] = 0;

      printf("Exception -> %s \n", exceptionSTR);
      printf("At: %d:%d", line, column);
    }
    

    switch (jsErrorCode) {
    case JsNoError:                            return "JsNoError";
    case JsErrorCategoryUsage:                 return "JsErrorCategoryUsage";
    case JsErrorInvalidArgument:               return "JsErrorInvalidArgument";
    case JsErrorNullArgument:                  return "JsErrorNullArgument";
    case JsErrorNoCurrentContext:              return "JsErrorNoCurrentContext";
    case JsErrorInExceptionState:              return "JsErrorInExceptionState";
    case JsErrorNotImplemented:                return "JsErrorNotImplemented";
    case JsErrorWrongThread:                   return "JsErrorWrongThread";
    case JsErrorRuntimeInUse:                  return "JsErrorRuntimeInUse";
    case JsErrorBadSerializedScript:           return "JsErrorBadSerializedScript";
    case JsErrorInDisabledState:               return "JsErrorInDisabledState";
    case JsErrorCannotDisableExecution:        return "JsErrorCannotDisableExecution";
    case JsErrorHeapEnumInProgress:            return "JsErrorHeapEnumInProgress";
    case JsErrorArgumentNotObject:             return "JsErrorArgumentNotObject";
    case JsErrorInProfileCallback:             return "JsErrorInProfileCallback";
    case JsErrorInThreadServiceCallback:       return "JsErrorInThreadServiceCallback";
    case JsErrorCannotSerializeDebugScript:    return "JsErrorCannotSerializeDebugScript";
    case JsErrorAlreadyDebuggingContext:       return "JsErrorAlreadyDebuggingContext";
    case JsErrorAlreadyProfilingContext:       return "JsErrorAlreadyProfilingContext";
    case JsErrorIdleNotEnabled:                return "JsErrorIdleNotEnabled";
    case JsCannotSetProjectionEnqueueCallback: return "JsCannotSetProjectionEnqueueCallback";
    case JsErrorCannotStartProjection:         return "JsErrorCannotStartProjection";
    case JsErrorInObjectBeforeCollectCallback: return "JsErrorInObjectBeforeCollectCallback";
    case JsErrorObjectNotInspectable:          return "JsErrorObjectNotInspectable";
    case JsErrorPropertyNotSymbol:             return "JsErrorPropertyNotSymbol";
    case JsErrorPropertyNotString:             return "JsErrorPropertyNotString";
    case JsErrorInvalidContext:                return "JsErrorInvalidContext";
    case JsInvalidModuleHostInfoKind:          return "JsInvalidModuleHostInfoKind";
    case JsErrorModuleParsed:                  return "JsErrorModuleParsed";
    // JsErrorCategoryEngine
    case JsErrorCategoryEngine:                return "JsErrorCategoryEngine";
    case JsErrorOutOfMemory:                   return "JsErrorOutOfMemory";
    case JsErrorBadFPUState:                   return "JsErrorBadFPUState";
    // JsErrorCategoryScript
    case JsErrorCategoryScript:                return "JsErrorCategoryScript";
    case JsErrorScriptException:               return "JsErrorScriptException";
    case JsErrorScriptCompile:                 return "JsErrorScriptCompile";
    case JsErrorScriptTerminated:              return "JsErrorScriptTerminated";
    case JsErrorScriptEvalDisabled:            return "JsErrorScriptEvalDisabled";
    // JsErrorCategoryFatal
    case JsErrorCategoryFatal:                 return "JsErrorCategoryFatal";
    case JsErrorFatal:                         return "JsErrorFatal";
    case JsErrorWrongRuntime:                  return "JsErrorWrongRuntime";
    // JsErrorCategoryDiagError
    case JsErrorCategoryDiagError:             return "JsErrorCategoryDiagError";
    case JsErrorDiagAlreadyInDebugMode:        return "JsErrorDiagAlreadyInDebugMode";
    case JsErrorDiagNotInDebugMode:            return "JsErrorDiagNotInDebugMode";
    case JsErrorDiagNotAtBreak:                return "JsErrorDiagNotAtBreak";
    case JsErrorDiagInvalidHandle:             return "JsErrorDiagInvalidHandle";
    case JsErrorDiagObjectNotFound:            return "JsErrorDiagObjectNotFound";
    case JsErrorDiagUnableToPerformAction:     return "JsErrorDiagUnableToPerformAction";
    default:
        return "<unknown>";
        break;
    }
}

static char* stringFromValue(JsValueRef value) {
  JsValueRef js_str;
  JsConvertValueToString(value, &js_str);
  char *str = nullptr;
  size_t str_length;
  JsCopyString(js_str, nullptr, 0, &str_length);  
  str = (char*) malloc(str_length + 1);
  JsCopyString(js_str, str, str_length + 1, nullptr);
  str[str_length] = 0;
  return str;
}

static bool createNamedFunction(const char* nameString, JsNativeFunction callback, JsValueRef* functionVar) {
    JsValueRef nameVar;
    FAIL_CHECK(JsCreateString(nameString, strlen(nameString), &nameVar));
    FAIL_CHECK(JsCreateNamedFunction(nameVar, callback, nullptr, functionVar));
    return true;
}

static bool installObjectsOnObject(JsValueRef object, const char* name, JsNativeFunction nativeFunction) {
    JsValueRef propertyValueRef;
    JsPropertyIdRef propertyId;
    FAIL_CHECK(JsCreatePropertyId(name, strlen(name), &propertyId));
    if (!createNamedFunction(name, nativeFunction, &propertyValueRef)) {
        return false;
    }
    FAIL_CHECK(JsSetProperty(object, propertyId, propertyValueRef, true));
    return true;
}
