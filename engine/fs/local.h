#pragma once

#include <fstream>

#include "fs/file_system.h"
#include "memory/input_stream.h"

namespace wage {
  namespace fs {

    class LocalFileInputStream : public memory::InputStream {
    public:
      LocalFileInputStream(std::string path) {
        file = std::fopen(path.c_str(), "rb");
      }

      ~LocalFileInputStream() {
        close();
      }

      virtual size_t read(memory::Byte* data, size_t size) {
        if (file)
          return std::fread(data, 1, size, file);
        else
          return -1;
      }

      virtual size_t seek(size_t position) {
        if (file) {
          if (std::fseek(file, static_cast<long>(position), SEEK_SET))
            return -1;
          return tell();
        }
        return -1;
      }

      virtual size_t tell() {
        if (file)
          return std::ftell(file);
        else
          return -1;
      }

      virtual size_t size() {
        if (file) {
          auto position = tell();
          std::fseek(file, 0, SEEK_END);
          auto size = tell();
          seek(position);
          return size;
        } else
          return -1;
      }

      virtual void close() {
        if (file)
          std::fclose(file);
      }

    private:
      std::FILE* file;
    };

    class Local : public FileSystem {

    public:
      Local(std::string base) : base({base}) {}

      ~Local() {}

      virtual memory::InputStream* readStream(File::Path path) const {
        return new LocalFileInputStream(fullPath(path));
      }

    private:
      std::string fullPath(File::Path relative) const {
        return base.append(relative).string();
      };

      File::Path base;
    };

  }
}
