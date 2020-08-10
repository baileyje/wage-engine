#pragma once

#include <string>

namespace wage {
  namespace ecs {

    class SystemContext;

    class System {

    public:
      System(){};

      virtual ~System(){};

      virtual void init(const SystemContext& context){};

      virtual void start(const SystemContext& context){};

      virtual void update(const SystemContext& context){};

      virtual void fixedUpdate(const SystemContext& context){};

      virtual void stop(const SystemContext& context){};

      virtual void deinit(const SystemContext& context){};
    };

  }
}
