#pragma once

#include <string>

#define LIFECYCLE_FUNC(Name) void Name(SystemContext* context);

namespace wage {
  class SystemContext;

  class System {

  public:
    System() {};

    virtual ~System(){};

    virtual void init(const SystemContext& context){};

    virtual void start(const SystemContext& context){};

    virtual void update(const SystemContext& context){};

    virtual void fixedUpdate(const SystemContext& context){};

    virtual void stop(const SystemContext& context){};

    virtual void deinit(const SystemContext& context){};

  };
}
