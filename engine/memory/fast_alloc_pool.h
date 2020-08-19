#pragma once

#include <vector>
#include <cassert>

#include "memory/allocator.h"
#include <iostream>

namespace wage {
  namespace memory {

    /**
   * Memory pool providing fixed size chunk allocations. Deallocated chunks are returned to the pool for reuse. Meant to provide fast 
   * allocation with no good means to iterate the data.
   */
    class Pool {

      class Iterator;

    public:
      struct Chunk {
        Chunk* next;
      };

      Pool(size_t chunkSize, int chunksPerBlock = 128, Allocator* allocator = Allocator::Permanent()) : chunkSize(chunkSize), chunksPerBlock(chunksPerBlock), allocator(allocator) {
      }

      void* allocate() {
        if (head == nullptr) {
          head = allocateBlock();
        }
        Chunk* freeChunk = head;
        head = head->next;
        return freeChunk;
      }

      void deallocate(void* chunk) {
        reinterpret_cast<Chunk*>(chunk)->next = head;
        head = reinterpret_cast<Chunk*>(chunk);
      }

    private:
      size_t chunkSize;

      int chunksPerBlock;

      Chunk* head = nullptr;

      Allocator* allocator;

      Chunk* allocateBlock() {
        size_t blockSize = chunksPerBlock * chunkSize;
        Chunk* blockBegin = reinterpret_cast<Chunk*>(allocator->allocate(blockSize));
        Chunk* chunk = blockBegin;
        for (int i = 0; i < chunksPerBlock - 1; ++i) {
          chunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + chunkSize);
          chunk = chunk->next;
        }
        chunk->next = nullptr;
        return blockBegin;
      }
    };
  }
}