#include "fs/local.h"

#include <fstream>
#include <iostream>

#include "memory/allocator.h"
#include "core/logger.h"

namespace wage {

  LocalFileSystem::LocalFileSystem(std::string base) : base(base) {
  }

  LocalFileSystem::~LocalFileSystem() {
  }

  std::string LocalFileSystem::fullPath(std::string relative) const {
    return base + "/" + relative;
  }

  std::unique_ptr<Buffer> LocalFileSystem::read(std::string path, Allocator* allocator) const {
    auto buffer = make<Buffer>();
    std::ifstream file(fullPath(path), std::ios::in | std::ios::binary);
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    Logger::info("Loading: ", path.c_str(), " -> ", size);
    file.seekg(0, std::ios::beg);
    buffer->allocate(size, allocator);
    file.read((char*)buffer->data(), size);
    file.close();
    return std::unique_ptr<Buffer>(buffer);
  }
}