#ifndef JSRT_MODULE_MANAGER_H
#define JSRT_MODULE_MANAGER_H

#include "ChakraCore.h"
#include <string>
#include <unordered_map>
#include <queue>

struct LoadTask {
  JsModuleRecord module;
  char* source;
  size_t sourceLength;
  JsSourceContext sourceContext;	
};

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
  
  JsModuleRecord createModule(std::string specifier, JsModuleRecord parentRecord, const std::string, bool *out_is_new);

  static ModuleManager* shared;

  static JsErrorCode CHAKRA_CALLBACK fetchImportedModule(JsModuleRecord importer, JsValueRef specifier, JsModuleRecord *outModule);

  static JsErrorCode CHAKRA_CALLBACK fetchDynamicImport(JsSourceContext importer, JsValueRef specifier, JsModuleRecord *outModule);
  
  static JsErrorCode CHAKRA_CALLBACK notifyModuleReady(JsModuleRecord module, JsValueRef exception);
  
  // TODO: JUNK HACK FEST 99
  char* readFile(std::string path);

  unsigned currentSourceContext;

private: 
  
  void parseModule(LoadTask* task);

  void evaluateModule(LoadTask* task);

  std::unordered_map<std::string, LoadedModule*> loadedModules;

  std::queue<LoadTask*> loadQueue;

  std::string moduleRoot;
};

#endif //JSRT_MODULE_MANAGER_H