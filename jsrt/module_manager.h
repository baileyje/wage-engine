#ifndef JSRT_MODULE_MANAGER_H
#define JSRT_MODULE_MANAGER_H

#include "ChakraCore.h"
#include <string>

#include "jsrt/load_queue.h"


struct LoadedModule {
	
  JsModuleRecord module;

	std::string path;

};

class ModuleManager {

public:
  
  ModuleManager(std::string moduleRoot);

  ~ModuleManager();

  void update();

  JsModuleRecord loadModule(JsModuleRecord importer, std::string path);
  
  JsModuleRecord createModule(std::string specifier, JsModuleRecord parentRecord, const char* url, bool *out_is_new);

  static ModuleManager* shared;

  static JsErrorCode CHAKRA_CALLBACK fetchImportedModule(JsModuleRecord importer, JsValueRef specifier, JsModuleRecord *outModule);

  static JsErrorCode CHAKRA_CALLBACK fetchDynamicImport(JsSourceContext importer, JsValueRef specifier, JsModuleRecord *outModule);
  
  static JsErrorCode CHAKRA_CALLBACK notifyModuleReady(JsModuleRecord module, JsValueRef exception);
  
  // TODO: JUNK HACK FEST 99
  char* readFile(std::string path);

  unsigned currentSourceContext;

private: 
  
  void parseModule(LoadQueue::Task* task);

  void evaluateModule(LoadQueue::Task* task);

  LoadedModule* loadedModules[128];

  int loadedModulesLength;

  LoadQueue* loadQueue;

  std::string moduleRoot;
};

#endif //JSRT_MODULE_MANAGER_H