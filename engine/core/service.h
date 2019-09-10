#pragma once

#include <string>

namespace wage { namespace core {

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

} }
