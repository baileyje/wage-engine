#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <vector>
#include <cassert>

namespace wage {

  template <typename T>
  class ObjectPool {
    friend class Reference;

  public:

    class Reference {

    public:

      Reference() : Reference(nullptr, OutOfBounds, 0) {
      }

      Reference(ObjectPool<T>* pool, size_t index, size_t version) : pool(pool), index(index), version(version) {
      }

      bool operator==(const Reference& other) const {
        return pool == other.pool && index == other.index;
      }
      
      bool operator!=(const Reference& other) const {
        return !operator==(other);
      }

      bool isValid() const {
        return pool != nullptr && pool->storage[index].valid && pool->storage[index].version == version;
      }

      operator bool() const { 
        return isValid(); 
      }

      T* operator->() {
        assert(isValid());
        T* item = pool->get(index);
        return item;
      }

      const T* operator->() const {
        return const_cast<Reference*>(this)->operator->();
      }

      T& operator*() {
        assert(isValid());
        T* item = pool->get(index);
        return *item;
      }

      const T& operator*() const {
        return const_cast<Reference*>(this)->operator*();
      }

      inline void free() const {
        pool->free(index);
      }

      inline size_t getIndex() {
        return index;
      }

      inline T* get() {
        assert(isValid());
        return pool->get(index);
      }

    private:
      
      ObjectPool<T>* pool;
      
      size_t index;

      size_t version;

    };

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
      
      typedef Reference& reference;

      typedef Reference* pointer;

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
        currentRef = Reference(pool, index, pool->storage[index].version);
        return (*this);
      }

      Iterator operator++(int) {
        Iterator temp = *this;
        ++(*this);
        return temp;
      }

      Iterator& operator--() {
        index = pool->storage[index].prev;
        currentRef = Reference(pool, index, pool->storage[index].version);
        return (*this);
      }

      Iterator operator--(int) {
        Iterator temp = *this;
        --(*this);
        return temp;
      }

      reference operator*() { return currentRef; }

      pointer operator->() { return currentRef; }

      Reference asReference() const {
        return currentRef;
      }

    private:
      
      ObjectPool<T>* pool;
      
      size_t index;

      Reference currentRef;
    };

    static const size_t head = 0;

    static const size_t tail = 1;

    static const size_t freeHead = 2;

    static const size_t freeTail = 3;
    
    static const size_t reserved = 4;

    ObjectPool() {
      clear();
    }

    void clear() {
      storage.resize(reserved);
      storage.reserve(1000); // TODO: Make configurable
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
    Reference create(Args... args) {
      size_t index = OutOfBounds;
      if (storage[freeTail].prev != freeHead) {
        index = storage[freeTail].prev;
        storage[index].version++;
        removeFromList(index);      
      } else {
        index = storage.size();
        storage.push_back(Node(index));
      }    
      addToList(index, tail);
      storage[index].valid = true;
      new (&(storage[index].item)) T(args...);
      return Reference(this, index, storage[index].version);
    }

    void removeFromList(size_t index) {
      storage[storage[index].prev].next = storage[index].next;
      storage[storage[index].next].prev = storage[index].prev;    
    }

    void free(size_t index) {
      storage[index].valid = false;
      storage[index].item.~T();
      new (&(storage[index].item)) T;
      removeFromList(index);
      addToList(index, freeTail);
    }

    Iterator begin() {
      return Iterator(this, storage[head].next);
    }

    Iterator end() {
      return Iterator(this, tail);
    }

    void debug() {
      printf("Head: %zu\n", head);
        printf("- prev: %zu\n", storage[head].prev);
        printf("- next: %zu\n", storage[head].next);
      for (auto& node : storage) {
        printf("Index: %zu\n", node.index);
        printf("- prev: %zu\n", node.prev);
        printf("- next: %zu\n", node.next);
      }
      printf("Tail: %zu\n", tail);
      printf("- prev: %zu\n", storage[tail].prev);
      printf("- next: %zu\n", storage[tail].next);
    }

  private:

    T* get(size_t index) {
      return index < storage.size() ? &storage[index].item : nullptr;
    }

    const T* get(size_t index) const {
      return index < storage.size() ? &storage[index].item : nullptr;
    }

    std::vector<Node> storage;

  };

}

#endif //OBJECT_POOL_H