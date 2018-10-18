#include "fs/local_file_system.h"

#include <fstream>

LocalFileSystem::LocalFileSystem(std::string base) : base(base) {
}

LocalFileSystem::~LocalFileSystem() {  
}

std::string* LocalFileSystem::read(std::string path) {
  std::string fullPath = base + "/" + path;
  std::ifstream ifs(fullPath);
  std::string* data = new std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
  return data;
}