#ifndef ASSET_BUFFER_H
#define ASSET_BUFFER_H

namespace wage {

  class Buffer {

  public:
  
    virtual const unsigned char* data() const = 0;

    virtual size_t length() const = 0;
  
  };

}

#endif //ASSET_BUFFER_H
