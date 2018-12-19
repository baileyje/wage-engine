#ifndef ASSET_BUFFER_H
#define ASSET_BUFFER_H

namespace wage {

  class Buffer {

  public:
  
    virtual unsigned char* data() = 0;

    virtual size_t length() = 0;
  
  };

}

#endif //ASSET_BUFFER_H
