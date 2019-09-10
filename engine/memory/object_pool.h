#pragma once

#include <vector>
#include <cassert>

#include "memory/allocator.h"
#include "memory/reference.h"

#include "memory/storage.h"
#include "memory/fixed_storage.h"
#include "memory/dynamic_storage.h"

namespace wage { namespace memory {

  template <typename T, typename IndexType = uint32_t>
  class ObjectPool : public Reference<T, IndexType>::Source {

    template <typename RT, typename RIndexType>
    friend class Reference;

    typedef Reference<T, IndexType> RefType;

  public:
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

      friend class ObjectPool<T, IndexType>;

      friend struct Item;

    public:
      Iterator(ObjectPool<T, IndexType>* pool, Item* current) : pool(pool), current(current) {}

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

      RefType operator*() {
        return pool->reference(current->index);
      }

      RefType operator->() {
        return pool->reference(current->index);
      }

    private:
      ObjectPool<T, IndexType>* pool;

      Item* current;
    };

    ObjectPool(int poolSize = 100) : storage(poolSize), poolSize(poolSize), currentSize(0) {
      clear();
    }

    template <typename... Args>
    RefType create(Args... args) {
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
      return reference(index);
    }

    RefType reference(IndexType index) {
      return RefType(this, index, storage[index].version);
    }

    T* get(RefType ref) {
      return get(ref.index());
    }

    bool valid(RefType ref) {
      auto& item = storage.get(ref.index());
      return ref.source() != nullptr && item.valid && item.version == ref.version();
    }

    void destroy(RefType ref) {
      if (valid(ref)) {
        destroy(ref.index());
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

    T* get(IndexType index) {
      return index < currentSize ? &storage[index].item : nullptr;
    }

    void destroy(IndexType index) {
      storage[index].valid = false;
      storage[index].item.~T();
      new (&(storage[index].item)) T;
      removeFromList(index);
      addToList(index, &freeTail);
    }

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

} }