#pragma once

#include <memory>

#include "memory/buffer.h"

namespace wage { namespace fs {

  class File {

  public:

    class ContentProvider {
    public:
      virtual std::unique_ptr<memory::Buffer> read(std::string path, memory::Allocator* allocator) const = 0;
    };

    File(std::string path, const ContentProvider* provider) : _path(path), provider(provider) {
    }

    inline std::string path() const {
      return _path;
    }

    std::unique_ptr<memory::Buffer> read(memory::Allocator* allocator) const {
      return provider->read(_path, allocator);
    }

  private:
    std::string _path;

    const ContentProvider* provider;
  };

} }