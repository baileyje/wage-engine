#pragma once

#include <cassert>
#include <vector>

#include "memory/storage.h"
#include "memory/allocator.h"

namespace wage { namespace memory {

  template <typename T, typename IndexType = size_t>
  class DynamicStorage : public Storage<T, IndexType> {

  public:
    DynamicStorage(int chunkSize = 100) : _chunkSize(chunkSize) { /* _currentSize(0)*/
      clear();
    }

    void clear() {
      // TODO. Something here would be good
    }

    T& get(IndexType index) {
      int chunkIndex = index / _chunkSize;
      auto chunk = chunkAt(chunkIndex);
      return chunk._storage[index % _chunkSize];
    }

  private:
    class Chunk {

      friend class DynamicStorage;

      Chunk() : _storage(nullptr), allocated(false) {}

      T* _storage;

      bool allocated;
    };

  private:
    Chunk& chunkAt(IndexType chunkIndex) {
      if (chunks.size() <= chunkIndex) {
        for (int i = 0; chunks.size() <= chunkIndex; ++i) {
          chunks.push_back({});
        }
      }
      if (!chunks[chunkIndex].allocated) {
        size_t allocSize = sizeof(T) * _chunkSize; // + sizeof(T) - 1;
        chunks[chunkIndex]._storage = (T*)Allocator::Permanent()->allocate(allocSize, alignof(T));
        chunks[chunkIndex].allocated = true;
      }
      return chunks[chunkIndex];
    }

    int _chunkSize;

    int _capacity;

    std::vector<Chunk> chunks;
  };

} }
