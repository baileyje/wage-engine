#pragma once

#include <fstream>

#include "fs/file_system.h"

namespace wage { namespace fs {

  class Local : public FileSystem {

  public:
    Local(std::string base) : base(base) {}

    ~Local() {}

    std::unique_ptr<memory::Buffer> read(std::string path, memory::Allocator* allocator) const override {
      auto buffer = memory::make<memory::Buffer>();
      std::ifstream file(fullPath(path), std::ios::in | std::ios::binary);
      file.seekg(0, std::ios::end);
      size_t size = file.tellg();
      core::Logger::debug("Loading: ", path.c_str(), " -> ", size);
      file.seekg(0, std::ios::beg);
      buffer->allocate(size, allocator);
      file.read((char*)buffer->data(), size);
      file.close();
      return std::unique_ptr<memory::Buffer>(buffer);
    }

  private:
    std::string fullPath(std::string relative) const {
      return base + "/" + relative;
    };

    std::string base;
  };

} }
