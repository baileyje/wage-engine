#pragma once

#include <string>

#include "core/service.h"
#include "memory/buffer.h"
#include "memory/allocator.h"

#include "fs/file.h"

namespace wage {
  namespace fs {

#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

    class FileSystem : public core::Service, public File::ContentProvider {

    public:
      FileSystem() : Service("FileSystem") {
      }

      ~FileSystem() {}

      inline File get(std::string path) const {
        return File(path, this);
      }

      inline std::string path(const std::initializer_list<std::string> parts) {
        std::string result = "";
        for (auto iter = parts.begin(); iter != parts.end(); ++iter) {
          result += *iter;
          if (iter != parts.end() - 1) {
            result += PATH_SEPARATOR;
          }
        }
        return result;
      }

      virtual memory::Buffer* read(std::string path, memory::Allocator* allocator) const = 0;
    };

  }
}
