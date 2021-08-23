#pragma once

#include <chrono>

#define MAX_SAMPLES 100
typedef std::chrono::high_resolution_clock::time_point TimePoint;

namespace wage {
  namespace util {

    class Timer {

    public:

      void tick() {
        auto newTime = std::chrono::high_resolution_clock::now();
        double elapsed = (std::chrono::duration_cast<std::chrono::milliseconds>(newTime - lastTime)).count();
        lastTime = newTime;
        frameSum -= frameTimes[frameIndex];
        frameSum += elapsed;
        frameTimes[frameIndex] = elapsed;
        frameIndex = (frameIndex + 1) % MAX_SAMPLES;
        rollingAverage = frameSum / MAX_SAMPLES;
      }

      inline double averageTime() {
        return rollingAverage;
      }

    private:
      TimePoint lastTime;
      int frameIndex = 0;
      double frameSum = 0;
      double frameTimes[MAX_SAMPLES];
      double rollingAverage;
    };

  }
}