#include "jsrt/module_manager.h"
#include "jsrt/util.h"

ModuleManager* ModuleManager::shared;

// TODO: Move to singleton
ModuleManager::ModuleManager(std::string moduleRoot) : moduleRoot(moduleRoot), currentSourceContext(0), loadedModulesLength(0), loadQueue(new LoadQueue()) {
}

ModuleManager::~ModuleManager() {
  delete loadQueue;
  for (int i = 0; i < loadedModulesLength; i++) {
    free(loadedModules[i]);
  }
}

void ModuleManager::update() {
  while(!loadQueue->empty()) {    
    LoadQueue::Task* task = loadQueue->pop();
    if (task->source != NULL) {
      parseModule(task);
      free(task->source);          
    } else {
      evaluateModule(task);
    }
    free(task);
  }
}

JsModuleRecord ModuleManager::loadModule(JsModuleRecord importer, std::string path) {
  // printf("Requested: %s\n", path);
  JsValueRef sepcifierRef;
  JsCreateString(path.c_str(), path.length(), &sepcifierRef);
  bool is_new_module;

  for (int i = 0; i < loadedModulesLength; i++) {
    LoadedModule* loaded = loadedModules[i];
    // printf("requesting: %s\n", path);
    // printf("checking: %s\n", loaded->path);    
    if (loaded->path.compare(path) == 0) {
      // printf("Returning same: %s\n", path);
      return loaded->module;
    }    
  }

  JsModuleRecord module_record = createModule(path, importer, path.c_str(), &is_new_module);
  
  LoadedModule* loaded = (LoadedModule*)malloc(sizeof(LoadedModule));
  loaded->module = module_record;
  loaded->path = path;  
  loadedModules[loadedModulesLength++] = loaded;
  // printf("ModuleRoot: %s\n", moduleRoot.c_str());
  // printf("ModulePath: %s\n", (moduleRoot +  path).c_str());
  char* source = readFile( (moduleRoot +  path).c_str() );
  // printf("SRC: %s\n", source);
  LoadQueue::Task* task = (LoadQueue::Task*)malloc(sizeof(LoadQueue::Task));  
  task->sourceContext = currentSourceContext++;
  task->module = module_record;
  task->source = source;
  task->sourceLength = strlen(source);
  loadQueue->push(task);  
  return module_record; 
}

JsModuleRecord ModuleManager::createModule(std::string specifier, JsModuleRecord parent_record, const char* url, bool *out_is_new) {
  // printf("Module: %s\n", specifier);
  JsModuleRecord module_record;
  JsValueRef     specifier_ref;
	JsValueRef     url_ref;
  *out_is_new = true;
	JsCreateString(specifier.c_str(), specifier.length(), &specifier_ref);
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

void ModuleManager::parseModule(LoadQueue::Task* task) {
  // printf("SRC %s\n", task->source);
  JsValueRef exception;  
  JsErrorCode  errorCode = JsParseModuleSource(task->module, task->sourceContext, (BYTE*)task->source, (unsigned int)task->sourceLength, JsParseModuleSourceFlags_DataIsUTF8, &exception);
  FAIL_CHECK(errorCode);
  // if (errorCode == JsErrorScriptCompile) {
  //   printException(exception);
  // } else {
    
  // }
}

void ModuleManager::evaluateModule(LoadQueue::Task* task) {
  JsValueRef result;
  FAIL_CHECK(JsModuleEvaluation(task->module, &result));  
}

JsErrorCode CHAKRA_CALLBACK ModuleManager::fetchImportedModule(JsModuleRecord importer, JsValueRef module_name, JsModuleRecord *outModule) {
  char* moduleName = stringFromValue(module_name); 
  // printf("Importing - %s\n", moduleName);
  JsModuleRecord module_record = ModuleManager::shared->loadModule(importer, moduleName);  
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
  LoadQueue::Task* task = (LoadQueue::Task*)malloc(sizeof(LoadQueue::Task));  
  task->module = module;
  task->source = NULL;
  ModuleManager::shared->loadQueue->push(task);    
	return JsNoError;
}

// TODO: JUNK HACK FEST 99
char* ModuleManager::readFile(std::string path) {
  FILE* f = fopen(path.c_str(), "r");
  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);
  char* data = new char[size];
  rewind(f);
  fread(data, sizeof(char), size, f);
  fclose(f);
  return data; 
}
