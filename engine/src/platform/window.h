#pragma once

namespace wage {
  namespace platform {

    /**
     * Window represents the actual visual window. It is created with generic properties, but primarily
     * wraps a platform specific window instance. 
     */
    class Window {

    public:
      /**
       * Construct a new window wrapper with platform specific window and attributes.
       */
      Window(void* wrapped, int width, int height, float scale = 1) : wrapped(wrapped), _width(width), _height(height), _scale(scale) {
      }

      virtual ~Window() {}

      /**
       * Get access to the platform specific window with a concrete template.
       */
      template <typename T>
      T* as() {
        return reinterpret_cast<T*>(wrapped);
      }

      /**
       * Get the current window with.
       */
      inline int width() const {
        return _width;
      }

      /**
       * Get the current window height.
       */
      inline int height() const {
        return _height;
      }

      /**
       * Get the current window scale.
       */
      inline int scale() const {
        return _scale;
      }

    private:
      void* wrapped;

      int _width;

      int _height;

      float _scale;
    };

  }
}