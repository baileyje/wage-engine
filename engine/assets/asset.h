#pragma once

#include <string>

#include "memory/buffer.h"

namespace wage { namespace assets {

  class Asset {

  public:
    typedef std::string Key;

    Asset(Key key) : _key(key), _loaded(false) {
    }

    virtual ~Asset() {}

    virtual Key key() const {
      return _key;
    }

    virtual bool loaded() const {
      return _loaded;
    }

    virtual void loaded(bool loaded) {
      _loaded = loaded;
    }

    virtual void set(std::shared_ptr<memory::Buffer> buffer) {
      this->buffer = buffer;
    }

    virtual void onLoad() {
    }

  protected:
    std::shared_ptr<memory::Buffer> buffer;

  private:
    Key _key;

    bool _loaded;
  };
} }
