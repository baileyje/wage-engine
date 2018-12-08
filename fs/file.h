#include <fstream>
#include <iterator>
#include <vector>

namespace wage {

  typedef std::vector<unsigned char> Buffer;

  class File {

  public:

    File(Buffer buffer) : _data(buffer) {      
    }

    File(std::ifstream& stream) : _data(std::istreambuf_iterator<char>(stream), {} ) {      
    }

    unsigned char* data() {
      return _data.data();
    }

    size_t length() {
      return _data.size();
    }

  private:

    Buffer _data;

  };

}