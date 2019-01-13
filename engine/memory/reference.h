#ifndef MEMORY_REFERENCE_H
#define MEMORY_REFERENCE_H

#include <cstddef>
#include <assert.h>  

namespace wage {
  
  template <typename T>
  class Reference {
  
  public:

    class Source {      

    public:  
      
      virtual bool isValid(Reference ref) const = 0;

      virtual void free(Reference ref) const = 0;

      virtual T* get(Reference ref) = 0;

    };

    static const size_t OutOfBounds = static_cast<size_t>(-1);

    Reference() : Reference(nullptr, OutOfBounds, 0) {
    }

    Reference(Source* source, size_t index, size_t version) : _source(source), _index(index), _version(version) {
    }

    virtual ~Reference() {}

    inline bool isValid() {
      return _source->isValid(*this);
    }

    virtual void free() const {
      _source->free(*this);
    };

    virtual T* get() {
      return _source->get(*this);
    }

    operator bool() const { 
      return isValid(); 
    }

    T* operator->() {
      assert(isValid());
      return get();
    }

    const T* operator->() const {
      return const_cast<Reference*>(this)->operator->();
    }

    T& operator*() {
      assert(isValid());
      return *get();
    }

    const T& operator*() const {
      return const_cast<Reference<T>*>(this)->operator*();
    }

    bool operator==(const Reference& other) const {
      return _source == other._source && _index == other._index;
    }
    
    bool operator!=(const Reference& other) const {
      return !operator==(other);
    }

    inline Source* source() {
      return _source;
    }

    inline size_t index() {
      return _index;
    }

    inline size_t version() {
      return _version;
    }

  private:
    
    Source* _source;
    
    size_t _index;

    size_t _version;
  
  };

}

#endif //MEMORY_REFERENCE_H