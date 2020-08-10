#pragma once

#include <vector>
#include <cassert>

#include "memory/allocator.h"
#include <iostream>

namespace wage {
  namespace memory {

    /**
     * Memory pool providing fixed size chunk allocations. Deallocated chunks are returned to the pool for reuse. The will grow
     * in predictable item blocks. Items are linked to allow iteration of active pool items.
     */
    class Pool {

    public:
      typedef void* Item;

      /**
       * We are going cheat and create a footer with a next and prev pointer and tack it onto the end of the data.
       */
      struct Footer {
        Item prev;
        Item next;
      };

      class Iterator {

        friend class Pool;

        friend struct Footer;

      public:
        Iterator(Pool* pool, Item current) : pool(pool), current(current) {}

        ~Iterator() {}

        bool operator==(const Iterator& other) const {
            return pool == other.pool && current == other.current;
        }

        bool operator!=(const Iterator& other) const {
            return !operator==(other);
        }

        Iterator& operator++() {
            current = pool->footer(current)->next;
            return (*this);
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        Iterator& operator--() {
            current = pool->footer(current)->prev;
            return (*this);
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --(*this);
            return temp;
        }

        Item operator*() {
            return current;
        }

        Item operator->() {
            return current;
        }

      private:
        Pool* pool;

        Item current;
      };

      /**
       * Create a pool with a fixed item size. Optionally the caller can control the block size and which allocator to use.
       */
      Pool(size_t _itemSize, int itemsPerBlock = 128, Allocator* allocator = Allocator::Permanent()) : _itemSize(_itemSize), itemsPerBlock(itemsPerBlock), allocator(allocator) {
      }

      /**
       * Allocate an items worth of memory from the pool. This will allocate a new block if needed.
       */
      Item allocate() {
        if (freeHead == nullptr) {
          freeHead = allocateBlock();
        }
        // Rip it off the free head and update the free head to the next in line
        Item freeItem = freeHead;
        auto newFooter = footer(freeItem);
        freeHead = footer(freeHead)->next;
        // Add it to the tail of the alloced list.
        newFooter->prev = allocedTail;
        newFooter->next = nullptr;
        if (allocedTail != nullptr) {
            footer(allocedTail)->next = freeItem;
        }
        allocedTail = freeItem;
        // Return the free item.
        _currentSize++;
        return freeItem;
      }
      /**
       * Deallocate an item once no longer needed.
       */
      void deallocate(Item item) {
        // Correct allocated chain
        Footer* itemFooter = footer(item);
        if (itemFooter->prev != nullptr) {
          footer(itemFooter->prev)->next = itemFooter->next;
        }
        if (itemFooter->next != nullptr) {
          footer(itemFooter->next)->prev = itemFooter->prev;
        }
        // Place this item at the head of the free queue
        if (item == allocedHead) {
          allocedHead = itemFooter->next;
        }
        if (item == allocedTail) {
          allocedTail = itemFooter->prev;
        }
        itemFooter->prev = nullptr;
        itemFooter->next = freeHead;
        freeHead = item;
        _currentSize--;
      }

      inline Iterator begin() {
        return Iterator(this, allocedHead);
      }

      inline Iterator end() {
        return Iterator(this, nullptr);
      }

      inline size_t currentSize() {
        return _currentSize;
      }

    private:
      size_t _itemSize;

      int itemsPerBlock;

      Item freeHead = nullptr;

      Item allocedHead = nullptr;

      Item allocedTail = nullptr;

      Allocator* allocator;

      size_t _currentSize = 0;

      Item allocateBlock() {
          size_t itemAndFooterSize = _itemSize + sizeof(Footer);
          size_t blockSize = itemsPerBlock * itemAndFooterSize;
          std::cout << "Allocating block... " << blockSize << std::endl;
          void* blockBegin = allocator->allocate(blockSize);
          Item item = blockBegin;
          for (int i = 0; i < itemsPerBlock - 1; ++i) {
              Footer* footer = reinterpret_cast<Footer*>(reinterpret_cast<char*>(item) + _itemSize);
              footer->next = reinterpret_cast<Footer*>(reinterpret_cast<char*>(item) + itemAndFooterSize);
              item = footer->next;
          }
          footer(item)->next = nullptr;
          if (allocedHead == nullptr) {
              allocedHead = blockBegin;
          }
          return blockBegin;
      }

      /**
       * Helper to jump to the footer.
       */
      Footer* footer(Item item) {
          return reinterpret_cast<Footer*>(reinterpret_cast<char*>(item) + _itemSize);
      }
    };
  }
}