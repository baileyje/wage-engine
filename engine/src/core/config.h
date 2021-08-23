#pragma once

namespace wage {
  namespace core {

    /**
     * The configuration for the engine core.
     */
    class Config {
    public:
      Config(double fixedTimeStep) : _fixedTimeStep(fixedTimeStep) {}

      /**
       * Get the fixed timestep for any physics based simulation.
       */
      inline double fixedTimeStep() {
        return _fixedTimeStep;
      }

    private:
      double _fixedTimeStep;
    };

  }
}