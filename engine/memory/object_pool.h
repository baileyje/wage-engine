#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <vector>
#include <cassert>

#include "memory/allocator.h"
#include "memory/reference.h"

#include "memory/pool_storage.h"
#include "memory/fixed_storage.h"
#include "memory/dynamic_storage.h"

namespace wage {

  template <typename T>
  class ObjectPool : public Reference<T>::Source {
    template<typename O>
    friend class Reference;

  public:

    static const size_t OutOfBounds = static_cast<size_t>(-1);

    struct Node {
      
      Node() : Node(OutOfBounds) {}

      Node(size_t index) : index(index), prev(OutOfBounds), next(OutOfBounds), valid(false), version(0) {}

      // Move
      Node& operator=(Node&& src) {
        prev = std::move(src.prev);
        next = std::move(src.next);      
        item = std::move(src.item);
        index = std::move(src.index);
        return *this;
      }

      // Copy
      Node(Node&& src) {
        prev = std::move(src.prev);
        next = std::move(src.next);      
        item = std::move(src.item);
        index = std::move(src.index);
      }

      size_t index;

      size_t prev;
      
      size_t next;
      
      T item;

      bool valid;

      size_t version;
    };

    class Iterator {
      
      typedef Reference<T>& reference;

      typedef Reference<T>* pointer;

      friend class ObjectPool<T>;

      friend struct Node;

    public:
      
      Iterator(ObjectPool<T>* pool, size_t index) : pool(pool), index(index), currentRef(pool, index, pool->storage[index].version) {}

      ~Iterator() {}

      bool operator==(const Iterator& other) const {
        return pool == other.pool && index == other.index;
      }

      bool operator!=(const Iterator& other) const {
        return !operator==(other);
      }

      Iterator& operator++() {
        index = pool->storage[index].next;
        currentRef = Reference<T>(pool, index, pool->storage[index].version);
        return (*this);
      }

      Iterator operator++(int) {
        Iterator temp = *this;
        ++(*this);
        return temp;
      }

      Iterator& operator--() {
        index = pool->storage[index].prev;
        currentRef = Reference<T>(pool, index, pool->storage[index].version);
        return (*this);
      }

      Iterator operator--(int) {
        Iterator temp = *this;
        --(*this);
        return temp;
      }

      reference operator*() { return currentRef; }

      pointer operator->() { return currentRef; }

      Reference<T> asReference() const {
        return currentRef;
      }

    private:
      
      ObjectPool<T>* pool;
      
      size_t index;

      Reference<T> currentRef;
    };

    static const size_t head = 0;

    static const size_t tail = 1;

    static const size_t freeHead = 2;

    static const size_t freeTail = 3;
    
    static const size_t reserved = 4;

    ObjectPool(int poolSize = 100) : storage(poolSize + reserved), poolSize(poolSize), currentSize(0) {
      clear();
    }

    void clear() {
      printf("Clear!\n");
      currentSize = reserved;
      // size_t allocSize = sizeof(T) * poolSize; // + sizeof(T) - 1;
      // storage = (Node*)Allocator::Permanent()->allocate(allocSize, alignof(T));      
      storage[head].next = tail;
      storage[tail].prev = head;
      storage[freeHead].next = freeTail;
      storage[freeTail].prev = freeHead;
    }

    void addToList(size_t index, size_t listTail) {
      assert(index >= reserved && listTail < reserved);
      size_t prev = storage[listTail].prev;
      storage[listTail].prev = index;
      storage[prev].next = index;
      storage[index].prev = prev;
      storage[index].next = listTail;
    }

    template <typename... Args>
    Reference<T> create(Args... args) {
      size_t index = OutOfBounds;
      if (storage[freeTail].prev != freeHead) {
        index = storage[freeTail].prev;
        storage[index].version++;
        removeFromList(index);      
      } else {
        index = currentSize++;
      }
      addToList(index, tail);
      storage[index].valid = true;
      new (&(storage[index].item)) T(args...);
      return Reference<T>(this, index, storage[index].version);
    }

    void removeFromList(size_t index) {
      storage[storage[index].prev].next = storage[index].next;
      storage[storage[index].next].prev = storage[index].prev;    
    }

    void free(Reference<T> ref) {
      if (isValid(ref)) {
        free(ref.index());
      }
    }

    void free(size_t index) {
      storage[index].valid = false;
      storage[index].item.~T();
      new (&(storage[index].item)) T;
      removeFromList(index);
      printf("Free!\n");
      addToList(index, freeTail);
    }

    Iterator begin() {
      return Iterator(this, storage[head].next);
    }

    Iterator end() {
      return Iterator(this, tail);
    }

    void debug() {
      // printf("Head: %zu\n", head);
      //   printf("- prev: %zu\n", storage[head].prev);
      //   printf("- next: %zu\n", storage[head].next);
      // for (int i = 0; i < currentSize; i++) {
      //   auto node = storage[i];
      //   printf("Index: %zu\n", node.index);
      //   printf("- prev: %zu\n", node.prev);
      //   printf("- next: %zu\n", node.next);
      // }
      // printf("Tail: %zu\n", tail);
      // printf("- prev: %zu\n", storage[tail].prev);
      // printf("- next: %zu\n", storage[tail].next);
    }

    T* get(Reference<T> ref) {
      return get(ref.index());
    }

    bool isValid(Reference<T> ref) {
      auto& node = storage.get(ref.index());
      return ref.source() != nullptr && node.valid && node.version == ref.version();
    }

  private:

    T* get(size_t index) {
      return index < currentSize ? &storage[index].item : nullptr;
    }

    const T* get(size_t index) const {
      return index < currentSize ? &storage[index].item : nullptr;
    }

    // FixedStorage<Node> storage;
    DynamicStorage<Node> storage;
    // Node* storage;
    
    int poolSize;

    int currentSize;

  };

}

#endif //OBJECT_POOL_H