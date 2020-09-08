#pragma once

#include <stddef.h>
#include <vector>

#include "memory/allocator.h"

namespace wage {
  namespace memory {

    // Note: Perhaps we can make this tunable in some smart way.
    typedef size_t IndexType;

    /**
     * Interface for a memory storage container that allows you to arbitrarily grab memory chunks by index.
     */
    template <typename T = void>
    class IndexedStorage {

    public:
      /**
       * Get the raw pointer to the memory at a give index. Note: it is up to the impl to decide if requesting an index
       * beyond the currently allocated memory space is an error or triggers an expansion.
       */
      virtual T* get(IndexType index) = 0;

      /**
     * Friendly accessor.
     */
      virtual T* operator[](size_t index) {
        return get(index);
      }
    };

    /**
     * Indexed storage that allocates blocks of fixed size chunks of memory and utilizes indexing to jump
     * to specific chunks. This implementation will self-grow as needed to support more chunks than its current
     * capacity.
     */
    template <typename T = void>
    class ChunkyIndexedStorage : public IndexedStorage<T> {

    public:
      // TODO:  How do we want to tune the chunks per.....
      ChunkyIndexedStorage(int chunksPerBlock = 128, Allocator* allocator = Allocator::Permanent()) : _chunkSize(sizeof(T)), _chunksPerBlock(chunksPerBlock), allocator(allocator) {
      }

      T* get(IndexType index) {
        auto blockIndex = index / _chunksPerBlock;
        auto block = blockAt(blockIndex);
        auto chunkIndex = index % _chunksPerBlock;
        if (block._storage != nullptr) {
          return reinterpret_cast<T*>(static_cast<char*>(block._storage) + chunkIndex * _chunkSize);
        }
        return nullptr;
      }

    private:
      class Block {

        friend class ChunkyIndexedStorage;

        Block() : _storage(nullptr), allocated(false) {}

        void* _storage;

        bool allocated;
      };

      Block& blockAt(IndexType blockIndex) {
        if (blocks.size() <= blockIndex) {
          for (int i = 0; blocks.size() <= blockIndex; ++i) {
            blocks.push_back({});
          }
        }
        if (!blocks[blockIndex].allocated) {
          size_t allocSize = _chunkSize * _chunksPerBlock;
          blocks[blockIndex]._storage = allocator->allocate(allocSize);
          blocks[blockIndex].allocated = true;
        }
        return blocks[blockIndex];
      }

      int _chunkSize;

      int _chunksPerBlock;

      std::vector<Block> blocks;

      Allocator* allocator;
    };
  }
}