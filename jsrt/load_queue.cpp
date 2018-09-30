#include "jsrt/load_queue.h"

LoadQueue::LoadQueue() {
}

LoadQueue::~LoadQueue() {
  while (tasks.empty()) {
    Task* task = tasks.front();
    tasks.pop();
    free(task);
  }
}

bool LoadQueue::empty() {
  return tasks.empty();
}

void LoadQueue::push(LoadQueue::Task* task) {
  tasks.push(task);
  // printf("Pushed: %d\n", tasks.size());
}

LoadQueue::Task* LoadQueue::pop() {
  Task* task = tasks.front();
  tasks.pop();
  // printf("Popped: %d\n", tasks.size());
  return task;
}