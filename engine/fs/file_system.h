#pragma once

#include <string>

#include "core/service.h"
#include "memory/buffer.h"
#include "memory/input_stream.h"
#include "memory/allocator.h"

#include "fs/file.h"

namespace wage {
  namespace fs {

    /**
     * Generic filesystem interface that encapsulates reading files from some media source not 
     * necessarily a true file system.
     */
    class FileSystem : public core::Service, public File::ContentProvider {

    public:
      FileSystem() : Service("FileSystem") {}

      ~FileSystem() {}

      /**
       * Get a file handle for a specified path.
       */
      inline File get(File::Path path) const {
        return File(path, this);
      }

      /**
       * Get a file handle for specified path segments.
       */
      inline File get(std::vector<std::string> pathSegments) const {
        return File({pathSegments}, this);
      }

      /**
       * Read the contents of a file path into a memory buffer intialized using the provided allocator.
       */
      inline memory::Buffer read(File::Path path, memory::Allocator* allocator) const {
        core::Logger::debug("Loading: ", path.string());
        auto stream = readStream(path);
        auto buffer = stream->read(allocator);
        delete stream;
        return buffer;
      }

      /**
       * Get the read stream for a specific path.
       */
      virtual memory::InputStream* readStream(File::Path path) const = 0;
    };

  }
}
