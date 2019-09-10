#pragma once

namespace wage { namespace platform {

  class Window {

  public:
    Window(void* wrapped, int width, int height, float scale = 1) : wrapped(wrapped), _width(width), _height(height), _scale(scale) {
    }

    virtual ~Window() {}

    template <typename T>
    T* as() {
      return reinterpret_cast<T*>(wrapped);
    }

    inline int width() const {
      return _width;
    }

    inline int height() const {
      return _height;
    }

    inline int scale() const {
      return _scale;
    }

  private:
    void* wrapped;

    int _width;

    int _height;

    float _scale;
  };

} }