
#include "core.h"
#include "signal.h"

Core core;

void intHandler(int);

// So far so dumb
int main() {
  signal(SIGINT, intHandler);  
  core.init();
  core.start();
  return 0;
}

void intHandler(int) { 
  core.stop();
}