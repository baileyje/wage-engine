#ifndef JSRT_MODULE_MANAGER_H
#define JSRT_MODULE_MANAGER_H

#include "ChakraCore.h"
#include <string>
#include <unordered_map>
#include <queue>

#include "fs/file_system.h"

namespace wage {

  struct LoadTask {
    JsModuleRecord module;
    std::unique_ptr<Buffer> source;    
    JsSourceContext sourceContext;	
  };

  struct LoadedModule {
    
    JsModuleRecord module;

    std::string path;

  };

  class ModuleManager {

  public:
    
    ModuleManager(FileSystem* fileSystem);

    ~ModuleManager();

    void update();

    JsModuleRecord loadModule(JsModuleRecord importer, std::string path);
    
    JsModuleRecord createModule(std::string specifier, JsModuleRecord parentRecord, const std::string, bool *out_is_new);

    static ModuleManager* shared;

    static JsErrorCode CHAKRA_CALLBACK fetchImportedModule(JsModuleRecord importer, JsValueRef specifier, JsModuleRecord *outModule);

    static JsErrorCode CHAKRA_CALLBACK fetchDynamicImport(JsSourceContext importer, JsValueRef specifier, JsModuleRecord *outModule);
    
    static JsErrorCode CHAKRA_CALLBACK notifyModuleReady(JsModuleRecord module, JsValueRef exception);

    unsigned currentSourceContext;

  private: 
    
    void parseModule(LoadTask* task);

    void evaluateModule(LoadTask* task);

    std::unordered_map<std::string, LoadedModule*> loadedModules;

    std::queue<LoadTask*> loadQueue;

    std::string moduleRoot;
    
    FileSystem* fileSystem;
  };

}

#endif //JSRT_MODULE_MANAGER_H