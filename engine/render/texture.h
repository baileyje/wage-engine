#pragma once

#include <string>

namespace wage {
  namespace render {

    /**
     * Simple texture with basically no options other than providing an image path to load.
     */
    // TODO:  De-jank this
    class Texture {

    public:
      static Texture Default;

      Texture() : _path("****INVALID****") {
      }

      Texture(std::string path) : _path(path) {
      }

      ~Texture() {}

      inline std::string path() {
        return _path;
      }

      inline std::string id() {
        return path();
      }

    private:
      std::string _path;
    };
  }
}