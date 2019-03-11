
#ifndef CORE_SERVICE_H
#define CORE_SERVICE_H

#include <string>

namespace wage {

  class Service {

  public:
    Service(std::string name) : _name(name){};

    virtual ~Service(){};

    virtual void start(){};

    virtual void stop(){};

    virtual std::string name() const { return _name; };

  private:
    std::string _name;
  };
}

#endif // CORE_SERVICE_H