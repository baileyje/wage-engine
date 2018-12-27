#ifndef FILE_SYSTEM_FILE_H
#define FILE_SYSTEM_FILE_H

#include <fstream>
#include <iterator>
#include <vector>

namespace wage {

  class File {

  public:

    typedef std::vector<unsigned char> Buffer;

    File(std::string path)  {
      std::ifstream stream(path, std::fstream::binary);
      _data = Buffer(std::istreambuf_iterator<char>(stream), {});
    }

    File(Buffer buffer) : _data(buffer) {      
    }

    File(std::ifstream& stream) : _data(std::istreambuf_iterator<char>(stream), {} ) {      
    }

    inline unsigned char* data() {
      return _data.data();
    }

    inline size_t length() {
      return _data.size();
    }

  private:

    Buffer _data;

  };

}

#endif //FILE_SYSTEM_FILE_H