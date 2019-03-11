
#ifndef CORE_SYSTEM_H
#define CORE_SYSTEM_H

#include <string>

#define LIFECYCLE_FUNC(Name) void Name(SystemContext* context);

namespace wage {
  class SystemContext;

  class System {

  public:
    System(std::string name) : _name(name){};

    virtual ~System(){};

    virtual void init(SystemContext* context){};

    virtual void start(SystemContext* context){};

    virtual void update(SystemContext* context){};

    virtual void fixedUpdate(SystemContext* context){};

    virtual void stop(SystemContext* context){};

    virtual void deinit(SystemContext* context){};

    virtual std::string name() { return _name; };

  private:
    std::string _name;
  };
}

#endif // CORE_SYSTEM_H