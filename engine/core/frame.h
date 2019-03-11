#ifndef CORE_FRAME_H
#define CORE_FRAME_H

namespace wage {

  class Frame {

    friend class Core;

  public:
    Frame() : _timeStep(1.0 / 60.0), _time(0), _deltaTime(0) {
    }

    inline double timeStep() const {
      return _timeStep;
    }

    inline double time() const {
      return _time;
    }

    inline double deltaTime() const {
      return _deltaTime;
    }

  private:
    double _timeStep;

    double _time;

    double _deltaTime;
  };
}

#endif // CORE_FRAME_H