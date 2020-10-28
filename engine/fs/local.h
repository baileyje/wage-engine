#pragma once

#include <fstream>
#include <filesystem>

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

      virtual bool exists(File::Path path) const {
        return std::filesystem::exists(fullPath(path));
      }

      /**
       * Read the contents of a file path into a memory buffer intialized using the provided allocator.
       */
      virtual void write(File::Path path, memory::Byte* data, size_t length) const {
        std::FILE* file = std::fopen(fullPath(path).c_str(), "wb");
        std::fwrite(data, length, 1, file);
        std::fclose(file);
      }

    private:
      std::string fullPath(File::Path relative) const {
        return base.append(relative).string();
      };

      File::Path base;
    };

  }
}
