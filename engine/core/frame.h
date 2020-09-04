#pragma once

#include <chrono>

#define DefaultFixedStep 1.0 / 60.0

namespace wage {
  namespace core {

    typedef std::chrono::high_resolution_clock::time_point TimePoint;

    /**
     * Class containing the time frame related details for a given game update loop.
     */
    class Frame {

    public:
      Frame() : Frame(DefaultFixedStep) {
      }

      /**
       * Create a new frame instanace with specific timestap.
       */
      Frame(double fixedTimeStep) : _fixedTimeStep(fixedTimeStep), _time(0), _deltaTime(0), _lastWallClock(now()) {}

      /**
       * Reset the game time frame back to zero and clear out any accumulated values.
       */
      inline void reset() {
        _deltaTime = 0;
        _time = 0;
        _lastWallClock = now();
      }

      /**
       * Pause the game time.
       */
      inline void pause() {
      }

      /**
       * UnPause the game time.
       */
      inline void unpause() {
        // TODO: figure out before pause.
        _lastWallClock = now();
      }

      /**
       * Get the fixed timestep for game simulation.
       */
      inline double fixedTimeStep() const {
        return _fixedTimeStep;
      }

      /**
       * Get the current game time.
       */
      inline double time() const {
        return _time;
      }

      /**
       * Get the elapsed time since the last game update.
       */
      inline double deltaTime() const {
        return _deltaTime;
      }

      /**
       * Move to the next update time frame. This sets a new game time as well as the delta from the last time frame.
       */
      inline void nextFrame() {
        auto currentWallClock = now();
        _deltaTime = (std::chrono::duration_cast<std::chrono::duration<double>>(currentWallClock - _lastWallClock)).count();
        _time += _deltaTime;
        _lastWallClock = currentWallClock;
      }

    private:
      inline TimePoint now() {
        return std::chrono::high_resolution_clock::now();
      }

      double _fixedTimeStep;

      double _time;

      double _deltaTime;

      TimePoint _lastWallClock;
    };

  }
}
