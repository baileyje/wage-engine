#ifndef ASSET_H
#define ASSET_H

#include <string>

#include "assets/buffer.h"

namespace wage {

  class Asset {

  public:

    typedef std::string Key;

    Asset(Key key) : _key(key), _loaded(false) {      
    }

    virtual Key key() {
      return _key;
    }

    virtual bool loaded() {
      return _loaded;
    }

    virtual Buffer* buffer() {
      return _buffer;
    };

    virtual void set(Buffer* buffer) {
      _buffer = buffer;
      onLoad(buffer);
      _loaded = true;
    }

    virtual void onLoad(Buffer* buffer) {      
    }

  private:
    
    Key _key;

    bool _loaded;

    Buffer* _buffer;

  };

}

#endif //ASSET_H