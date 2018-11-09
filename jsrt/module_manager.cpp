#include "jsrt/module_manager.h"
#include "jsrt/util.h"

#include "core/logger.h"

ModuleManager* ModuleManager::shared;

// TODO: Move to singleton
ModuleManager::ModuleManager(FileSystem* fileSystem) : currentSourceContext(0), moduleRoot("engine/"), fileSystem(fileSystem) {
}

ModuleManager::~ModuleManager() {
  for (const auto &pair : loadedModules) {
    free(pair.second);
  }
}

void ModuleManager::update() {
  while(!loadQueue.empty()) {    
    LoadTask* task = loadQueue.front();
    loadQueue.pop();
    if (task->source != NULL) {
      parseModule(task);
      free((char*)task->source);          
    } else {
      evaluateModule(task);
    }
    free(task);
  }
}

JsModuleRecord ModuleManager::loadModule(JsModuleRecord importer, std::string path) {
  Logger::debug("Requested: %s\n", path.c_str());
  bool is_new_module;

  LoadedModule* loaded = loadedModules[path];
  if (loaded != NULL) {
    return loaded;
  }
  JsModuleRecord moduleRecord = createModule(path, importer, path, &is_new_module);  
  loaded = (LoadedModule*)malloc(sizeof(LoadedModule));
  loaded->module = moduleRecord;
  loaded->path = path;  
  loadedModules[path] = loaded;
  Logger::debug("ModulePath: %s\n", (moduleRoot +  path).c_str());
  const char* source = fileSystem->read(moduleRoot +  path);
  Logger::debug("SRC: %s\n", source);
  LoadTask* task = (LoadTask*)malloc(sizeof(LoadTask));  
  task->sourceContext = currentSourceContext++;
  task->module = moduleRecord;
  task->source = source;
  task->sourceLength = strlen(source); 
  loadQueue.push(task);  
  return moduleRecord; 
}

JsModuleRecord ModuleManager::createModule(std::string specifier, JsModuleRecord parentRecord, std::string url, bool *out_is_new) {
  Logger::debug("Creating Module: %s\n", specifier.c_str());
  JsModuleRecord moduleRecord;  	
  *out_is_new = true;  
  JsValueRef sepcifierRef = valueFromString(specifier);  
  // JsValueRef urlRef = valueFromString(url);
  JsInitializeModuleRecord(parentRecord, sepcifierRef, &moduleRecord);
  JsSetModuleHostInfo(moduleRecord, JsModuleHostInfo_HostDefined, sepcifierRef);
  // Setup callbacks
  JsSetModuleHostInfo(moduleRecord, JsModuleHostInfo_FetchImportedModuleCallback, (void*)ModuleManager::fetchImportedModule);
  JsSetModuleHostInfo(moduleRecord, JsModuleHostInfo_FetchImportedModuleFromScriptCallback, (void*)ModuleManager::fetchDynamicImport);
  JsSetModuleHostInfo(moduleRecord, JsModuleHostInfo_NotifyModuleReadyCallback, (void*)ModuleManager::notifyModuleReady);
  JsAddRef(moduleRecord, NULL);
  return moduleRecord;
}

void ModuleManager::parseModule(LoadTask* task) {
  Logger::debug("Paring source:\n %s\n", task->source);
  JsValueRef exception;  
  JsErrorCode  errorCode = JsParseModuleSource(task->module, task->sourceContext, (BYTE*)task->source, (unsigned int)task->sourceLength, JsParseModuleSourceFlags_DataIsUTF8, &exception);  
  if (errorCode == JsErrorScriptCompile) {
    printException(errorCode, exception);
  } else {
    FAIL_CHECK(errorCode);  
  }
}

void ModuleManager::evaluateModule(LoadTask* task) {
  JsValueRef result;
  FAIL_CHECK(JsModuleEvaluation(task->module, &result));  
}

JsErrorCode CHAKRA_CALLBACK ModuleManager::fetchImportedModule(JsModuleRecord importer, JsValueRef fetchName, JsModuleRecord *outModule) {
  char* moduleName = stringFromValue(fetchName);
  Logger::debug("Importing - %s", moduleName);
  JsModuleRecord moduleRecord = ModuleManager::shared->loadModule(importer, moduleName);  
  *outModule = moduleRecord;
  free(moduleName);
  return JsNoError;
}

JsErrorCode CHAKRA_CALLBACK ModuleManager::fetchDynamicImport(JsSourceContext importer, JsValueRef specifier, JsModuleRecord *outModule) {
  Logger::debug("Dynamic Import - %s", stringFromValue(specifier));
	return fetchImportedModule(NULL, specifier, outModule);
}
  
JsErrorCode CHAKRA_CALLBACK ModuleManager::notifyModuleReady(JsModuleRecord module, JsValueRef exception) {
  Logger::debug("Module Ready");
  LoadTask* task = (LoadTask*)malloc(sizeof(LoadTask));  
  task->module = module;
  task->source = NULL;
  ModuleManager::shared->loadQueue.push(task);    
	return JsNoError;
}

