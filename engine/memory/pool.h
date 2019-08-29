#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <vector>
#include <cassert>

#include "memory/allocator.h"

#include "memory/storage.h"
#include "memory/fixed_storage.h"
#include "memory/dynamic_storage.h"

namespace wage {


  template <typename T, typename IndexType = uint32_t>
  class Pool {

  public:

    class PoolIndex {
    public:

      PoolIndex(IndexType index, size_t version): _index(index), _version(version) {}

      inline IndexType index() const {
        return _index;
      }

      inline size_t version() const {
        return _version;
      }

    private:

      IndexType _index;

      size_t _version;

    };

    static const IndexType OutOfBounds = static_cast<IndexType>(-1);

    struct Item {

      Item() : Item(OutOfBounds) {}

      Item(IndexType index) : index(index), prev(nullptr), next(nullptr), valid(false), version(0) {}

      // Move
      Item& operator=(Item&& src) {
        prev = std::move(src.prev);
        next = std::move(src.next);
        item = std::move(src.item);
        index = std::move(src.index);
        return *this;
      }

      // Copy
      Item(Item&& src) {
        prev = std::move(src.prev);
        next = std::move(src.next);
        item = std::move(src.item);
        index = std::move(src.index);
      }

      IndexType index;

      Item* prev;

      Item* next;

      T item;

      bool valid;

      size_t version;
    };

    class Iterator {

      friend class Pool<T, IndexType>;

      friend struct Item;

    public:
      Iterator(Pool<T, IndexType>* pool, Item* current) : pool(pool), current(current) {}

      ~Iterator() {}

      bool operator==(const Iterator& other) const {
        return pool == other.pool && current == other.current;
      }

      bool operator!=(const Iterator& other) const {
        return !operator==(other);
      }

      Iterator& operator++() {
        current = current->next;
        return (*this);
      }

      Iterator operator++(int) {
        Iterator temp = *this;
        ++(*this);
        return temp;
      }

      Iterator& operator--() {
        current = current->prev;
        return (*this);
      }

      Iterator operator--(int) {
        Iterator temp = *this;
        --(*this);
        return temp;
      }

      PoolIndex operator*() {
        return {current->index, current->version};
      }

      PoolIndex operator->() {
        return {current->index, current->version};
      }

    private:
      Pool<T, IndexType>* pool;

      Item* current;
    };

    Pool(int poolSize = 100) : storage(poolSize), poolSize(poolSize), currentSize(0) {
      clear();
    }

    template <typename... Args>
    PoolIndex create(Args... args) {
      IndexType index = OutOfBounds;
      if (freeTail.prev != &freeHead) {
        index = freeTail.prev->index;
        storage[index].version++;
        removeFromList(index);
      } else {
        index = currentSize++;
      }
      addToList(index, &tail);
      storage[index].index = index;
      storage[index].valid = true;
      new (&(storage[index].item)) T(args...);
      return {storage[index].index, storage[index].version};
    }

    T* get(PoolIndex index) {
      return valid(index) ? &storage[index.index()].item : nullptr;
    }

    bool valid(PoolIndex index) {
      auto& item = storage.get(index.index());
      return item.valid && index.version() == item.version;
    }

    void destroy(PoolIndex index) {
      if (valid(index)) {
        storage[index.index()].valid = false;
        storage[index.index()].item.~T();
        new (&(storage[index.index()].item)) T;
        removeFromList(index.index());
        addToList(index.index(), &freeTail);
      }
    }

    Iterator begin() {
      return Iterator(this, head.next);
    }

    Iterator end() {
      return Iterator(this, &tail);
    }

    void clear() {
      storage.clear();
      currentSize = 0;
      head.next = &tail;
      tail.prev = &head;
      freeHead.next = &freeTail;
      freeTail.prev = &freeHead;
    }

  private:

    void addToList(IndexType index, Item* listTail) {
      auto item = &storage[index];
      auto prev = listTail->prev;
      listTail->prev = item;
      prev->next = item;
      item->prev = prev;
      item->next = listTail;
    }

    void removeFromList(IndexType index) {
      auto prev = storage[index].prev;
      auto next = storage[index].next;
      prev->next = next;
      next->prev = prev;
    }

    // FixedStorage<Item> storage;
    DynamicStorage<Item> storage;

    IndexType poolSize;

    IndexType currentSize;

    Item head;

    Item tail;

    Item freeHead;

    Item freeTail;
  };
}

#endif //MEMORY_POOL_H