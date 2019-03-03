#ifndef FS_ASSET_H
#define FS_ASSET_H


#include "fs/file.h"
#include "assets/asset.h"

namespace wage {

  class FileBuffer : public Buffer {
    
  public:

    FileBuffer(File* file) : _file(file) {      
    }

    inline const unsigned char* data() const {
      return file()->data();
    }

    inline size_t length() const {
      return file()->length();
    }

    inline const File* file() const {
      return _file;
    }

  private:

    File* _file;
  
  };

}

#endif //FS_ASSET_H