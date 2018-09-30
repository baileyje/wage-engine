#ifndef JSRT_LOAD_QUEUE_H
#define JSRT_LOAD_QUEUE_H

#include "ChakraCore.h"
#include <string>
#include <queue>

class LoadQueue {

public: 
  
  struct Task {
    JsModuleRecord module;
    char* source;
    size_t sourceLength;
    JsSourceContext sourceContext;	
  };

  LoadQueue();

  ~LoadQueue();

  void push(Task* task);

  Task* pop();

  bool empty();

private:
  
  std::queue<Task*> tasks;

};

#endif // JSRT_LOAD_QUEUE_H