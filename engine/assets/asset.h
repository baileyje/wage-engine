#pragma once

#include <string>

#include "memory/buffer.h"

namespace wage {
  namespace assets {

    class Asset {

    public:
      typedef std::string Key;

      typedef std::string Type;

      Asset(Type type, Key key) : _loaded(false), _type(type), _key(key) {
      }

      virtual ~Asset() {}

      virtual Type type() const {
        return _type;
      }

      virtual Key key() const {
        return _key;
      }

      virtual bool loaded() const {
        return _loaded;
      }

      virtual void loaded(bool loaded) {
        _loaded = loaded;
      }

      virtual void set(memory::Buffer* buffer) {
        this->buffer = buffer;
      }

      virtual void onLoad() {
      }

    protected:
      memory::Buffer* buffer;

      bool _loaded;

    private:
      Type _type;

      Key _key;
    };
  }
}
