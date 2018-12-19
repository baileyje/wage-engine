#ifndef ASSET_H
#define ASSET_H

#include <string>

#include "assets/buffer.h"

namespace wage {

  class Asset {

  public:

    typedef std::string Key;

    Asset(Key key) : key_(key) {      
    }

    virtual Key key() {
      return key_;
    }

    virtual bool loaded() {
      return loaded_;
    }

    virtual Buffer* buffer() {
      return buffer_;
    };

    virtual void set(Buffer* buffer) {
      buffer_ = buffer;
      onLoad(buffer);
      loaded_ = true;
    }

    virtual void onLoad(Buffer* buffer) {      
    }

  private:
    
    Key key_;

    bool loaded_;

    Buffer* buffer_;

  };

}

#endif //ASSET_H