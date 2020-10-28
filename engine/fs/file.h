#pragma once

#include <vector>

#include <memory>

#include "memory/buffer.h"
#include "memory/input_stream.h"

#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

namespace wage {
  namespace fs {

    /**
     * Handle object that represents a block of content on some type of filesystem. 
     * This does not hold the content of the file, but simply allows access to the content.
     */
    class File {

    public:
      /**
       * Convenience object to wrap file path segments. This provides utilities to
       * manipulate path structures and will generate correct file paths for a given system.
       */
      class Path {

      public:
        Path(const std::vector<std::string> segments) : _segments(segments) {
        }

        /**
         * Generate a new `Path` object by appending a child path to this path.
         */
        inline Path append(Path path) const {
          std::vector<std::string> newSegments;
          std::copy(_segments.begin(), _segments.end(), std::back_inserter(newSegments));
          std::copy(path._segments.begin(), path._segments.end(), std::back_inserter(newSegments));
          return {newSegments};
        }

        /**
         * Generate the system appropriate path string from the current segments.
         */
        inline std::string string() {
          std::string result = "";
          for (auto iter = _segments.begin(); iter != _segments.end(); ++iter) {
            result += *iter;
            if (iter != _segments.end() - 1) {
              result += PATH_SEPARATOR;
            }
          }
          return result;
        }

      private:
        std::vector<std::string> _segments;
      };

      /**
       * Virtual class providing the interface for how file contents can be loaded when needed.
       */
      class ContentProvider {
      public:
        /**
         * Read the contents of a file at the provide into an allocated memory buffer.
         */
        virtual memory::Buffer read(Path path, memory::Allocator* allocator) const = 0;

        /**
         * Determine if a file exists at the provided path.
         */
        virtual bool exists(Path path) const = 0;
      };

      /**
       * Create a file handle at a specific path with a content provider for future reading.
       */
      File(Path path, const ContentProvider* provider) : _path(path), provider(provider) {}

      /** 
       * Get the file path.
       */
      inline Path path() const {
        return _path;
      }

      /**
       * Does this file exist.
       */
      inline bool exists() const {
        return provider->exists(_path);
      }

      /**
       * Read the file into a memory block using the provided allocator.
       */
      memory::Buffer read(memory::Allocator* allocator) const {
        return provider->read(_path, allocator);
      }

    private:
      Path _path;

      const ContentProvider* provider;
    };

  }
}