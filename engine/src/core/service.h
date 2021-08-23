#pragma once

#include <string>

namespace wage {
  namespace core {

    /**
     * A service is an object that can be added to the engine core to add additional functionality. The only true lifecycle for a service is a 
     * start and stop method invoked at the start and stop of the game. The service can elect to receive additional events from the engine
     * by registering itself as a listener to one of the onUpdate, onFixedUpdate, etc. method callback. It should register itself as part of
     * its startup.
     */
    class Service {

    public:
      /**
       * Create a service with a name. The name provides additional context when logging and setting up the engine.
       */
      Service(std::string name) : _name(name){};

      virtual ~Service(){};

      /**
       * Start the service and do anything that is required to get the game service ready to support the game.
       */
      virtual void start(){};

      /**
       * Stop the service and cleanup anything that is no longer needed.
       */
      virtual void stop(){};

      /**
       * Pause the service.
       */
      virtual void pause(){};

      /**
       * Unpause the service.
       */
      virtual void unpause(){};

      /**
       * Reset the service to an initial state.
       */
      virtual void reset(){};

      /**
       * Get the name of the service.
       */
      virtual std::string name() const { return _name; };

    private:
      std::string _name;
    };

  }
}
