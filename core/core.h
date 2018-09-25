#ifndef CORE_H
#define CORE_H

class Jsrt;

class Core {

public:
  
  Core();

  ~Core();
  
  void init();
  
  void start();
  
  void stop();  

private:
  Jsrt* jsrt;
  
  bool running;
};

#endif // CORE_H