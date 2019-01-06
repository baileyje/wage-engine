#ifndef FS_ASSET_H
#define FS_ASSET_H


#include "engine/fs/file.h"
#include "engine/assets/asset.h"

namespace wage {

  class FileBuffer : public Buffer {
    
  public:

    FileBuffer(File* file) : file_(file) {      
    }

    inline unsigned char* data() {
      return file()->data();
    }

    inline size_t length() {
      return file()->length();
    }

    inline File* file() {
      return file_;
    }

  private:

    File* file_;
  
  };

}

#endif //FS_ASSET_H