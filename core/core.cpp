#include "core.h"
#include "jsrt.h"
#include <iostream>


Core::Core() : jsrt(new Jsrt()), running(false) {
}

Core::~Core() {
  delete jsrt;
}

void Core::init() {
  printf("Initializing WAGE Core.\n");
  jsrt->init();
}

void Core::start() {  
  if (running) {
    return;
  }
  printf("Starting WAGE Core.\n");
  running = true;  
  while (running) {
    jsrt->update();
  }  
}

void Core::stop() {
  if(!running) {
    return;
  }
  printf("Stopping WAGE Core.\n");
  running = false;
  
}