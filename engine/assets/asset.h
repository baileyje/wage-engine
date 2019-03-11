#ifndef ASSET_H
#define ASSET_H

#include <string>

#include "memory/buffer.h"

namespace wage {

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

    virtual void set(std::shared_ptr<Buffer> buffer) {
      printf("Set Called: %s\n", _key.c_str());
      this->buffer = buffer;
    }

    virtual void onLoad() {
    }

  protected:
    std::shared_ptr<Buffer> buffer;

  private:
    Key _key;

    bool _loaded;
  };
}

#endif //ASSET_H